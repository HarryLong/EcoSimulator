#include "simulator_manager.h"

#include <iostream>
#include "plant.h"
#include <algorithm>

#include "debuger.h"
#include <mutex>

static QRgb s_black_color_rgb(QColor(Qt::GlobalColor::black).rgb());

SimulatorManager::SimulatorManager() : m_time_keeper(), m_plant_storage(), m_elapsed_months(0), m_state(Stopped),
    m_environment_mgr(), m_plant_rendering_data(), m_plant_factory(), m_simulation_options(NULL)
{
    m_time_keeper.addListener(this);
}

SimulatorManager::~SimulatorManager()
{
    if(m_simulation_options)
        delete m_simulation_options;
}

void SimulatorManager::add_plant(Plant * p_plant)
{
    m_plant_storage.add(p_plant);
    m_environment_mgr.updateEnvironment(p_plant->m_center_position, p_plant->getCanopyWidth(), p_plant->getHeight(), p_plant->getRootSize(),
                                        p_plant->m_unique_id, p_plant->getMinimumSoilHumidityRequirement()); // Update resources in environment

    emit newPlant(p_plant->m_specie_name, p_plant->getColor());
}

void SimulatorManager::remove_plant(Plant * p)
{
    m_plant_storage.remove(p);
}

void SimulatorManager::start( SimulationConfiguration configuration)
{
    m_environment_mgr.setEnvironmentProperties(configuration.min_illumination, configuration.illumination_variance,
                                               configuration.min_humidity, configuration.humidity_variance,
                                               configuration.min_temperature, configuration.temperature_variance);
    m_simulation_options = new SimulationOptions(configuration.simulation_options);

    for(auto specie_it(configuration.m_plants_to_generate.begin()); specie_it != configuration.m_plants_to_generate.end(); specie_it++)
    {
        for(int plant_count(0); plant_count < specie_it->second; plant_count++)
        {
            add_plant(m_plant_factory.generate(specie_it->first));
        }
    }

    m_state = Running;
    m_stopping.store(false);
    m_time_keeper.start();
}

void SimulatorManager::resume()
{
    m_state = Running;
    m_stopping.store(false);
    m_time_keeper.start();
}

void SimulatorManager::pause()
{
    m_state = Paused;
    m_stopping.store(true);
    m_time_keeper.stop();
}

void SimulatorManager::stop()
{
    m_state = Stopped;
    m_stopping.store(true);
    m_time_keeper.stop();

    std::this_thread::sleep_for(std::chrono::milliseconds(250)); // Wait a delay to ensure all data processing has stopped from the previous trigger

    m_environment_mgr.reset();
    m_plant_rendering_data.clear();
    m_plant_storage.clear();
    m_elapsed_months = 0;

    emit update();// In order to remove on screen elements
}

void SimulatorManager::trigger()
{
    if(m_stopping.load())
        return;

    m_elapsed_months++;

    m_plant_storage.lock();

    std::vector<Plant*> plants(m_plant_storage.getPlants());
    /*
     * ITERATION # 1:
     * - Based on environmental factors calculates the strength of each plant
     * - Checks the status of the plant and acts accordingly (dies or grows)
     */
    int month((m_elapsed_months%12) + 1);
    m_environment_mgr.setMonth(month);

    for(Plant * p : plants)
    {
        // Shade
        int daily_illumination(m_environment_mgr.getDailyIllumination(month, p->m_center_position, p->getCanopyWidth(), p->getHeight()));

        // Humidity
        int soil_humidity(m_environment_mgr.getSoilHumidityPercentage(p->m_center_position, p->getRootSize(), p->m_unique_id));

        // Temperature [This is static but refetched each time in case it gets dynamic]
        int temp(m_environment_mgr.getTemperature(month, p->m_center_position));

        p->calculateStrength(daily_illumination, soil_humidity, temp);
    }

    // Check plant status
    std::vector<Plant *> plants_to_remove;

    /*
     * ITERATION # 1:
     * - Based on environmental factors calculates the strength of each plant
     * - Checks the status of the plant and acts accordingly (dies or grows)
     * - Updates the environment with the effect of the plant growth
     */
    for(Plant * p : plants)
    {
        PlantStatus status (p->getStatus());
        if(status == Alive)
        {
            p->newMonth();

            // Update the environment
            m_environment_mgr.updateEnvironment(p->m_center_position, p->getCanopyWidth(), p->getHeight(),
                                                p->getRootSize(), p->m_unique_id, p->getMinimumSoilHumidityRequirement());
        }
        else
        {
            plants_to_remove.push_back(p);
            m_environment_mgr.remove(p->m_center_position, p->getCanopyWidth(), p->getRootSize(), p->m_unique_id);
            emit removedPlant(p->m_specie_name, plant_status_to_string(status));
        }
    }

    // Remove the plants
    for(Plant * p : plants_to_remove)
    {
        remove_plant(p);
    }

    // Seeding
    if(m_simulation_options->seeding_enabled && m_elapsed_months % 12 == 6)
    {
        PlantStorageStructure species(m_plant_storage.getSpecies());

        for(auto specie(species.begin()); specie != species.end(); specie++)
        {
            int specie_id(specie->first);
            int specie_seed_count(m_plant_factory.getSpecieProperties(specie_id)->seeding_properties->seed_count);

            if(specie->second.size() > 0)
            {
                std::vector<Plant*> seeding_plants(m_plant_storage.getOnePlantPerCell(specie_id));

                auto plant_it(seeding_plants.begin());

                int seed_count(0);
                while(seed_count++ < specie_seed_count)
                {
                    Plant * seeding_plant (*plant_it);
                    QPoint position (seeding_plant->seed(1).at(0));
                    if(position.x() >= 0 && position.x() < AREA_WIDTH_HEIGHT &&
                        position.y() >= 0 && position.y() < AREA_WIDTH_HEIGHT)
                    {
                        add_plant(m_plant_factory.generate(specie_id, position));
                    }

                    if(++plant_it == seeding_plants.end())
                        plant_it = seeding_plants.begin();
                }
            }
            else
            {
                for(int i(0); i < specie_seed_count; i++)
                    add_plant(m_plant_factory.generate(specie_id));
            }
        }
    }

    m_plant_storage.unlock();
    refresh_rendering_data();

    emit update();
}

const PlantRenderDataContainer& SimulatorManager::getPlantRenderingData()
{
    return m_plant_rendering_data;
}

const EnvironmentSpatialHashMap & SimulatorManager::getEnvironmentRenderingData()
{
    return m_environment_mgr.getRenderingData();
}

void SimulatorManager::setMonthlyTriggerFrequency(int p_frequency)
{
    m_time_keeper.setUnitTime(p_frequency);
}

QString SimulatorManager::plant_status_to_string(PlantStatus status)
{
    switch (status){
        case DeathByAge:
            return QString("Age");
        case DeathByUnderIllumination:
            return  QString("Insufficient sunlight");
        case DeathByOverIllumination:
            return  QString("Too much sunlight");
        case DeathByFlood:
            return  QString("Flooded");
        case DeathByDrought:
            return  QString("Drought");
        case DeathByCold:
            return QString("Cold");
        case DeathByHeat:
            return QString("Heat");
    default:
        return QString("This is a bug!");
    }
}

void SimulatorManager::refresh_rendering_data()
{
    m_plant_rendering_data.lock();
    m_plant_rendering_data.clear();

    for(Plant * p : m_plant_storage.getSortedPlants(SortingCriteria::Height))
    {
        m_plant_rendering_data.push_back(
                    PlantRenderingData(p->m_specie_name, p->getColor(), p->m_center_position, p->getHeight(), p->getCanopyWidth(), p->getRootSize()));
    }

    m_plant_rendering_data.unlock();
}

void SimulatorManager::generateSnapshot()
{
    new std::thread(&PlantStorage::generateSnapshot, &m_plant_storage);
}

void SimulatorManager::generateStatisticalSnapshot()
{
    new std::thread(&PlantStorage::generateStatisticalSnapshot, &m_plant_storage);
}
