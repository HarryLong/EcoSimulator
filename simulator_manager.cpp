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
    m_environment_mgr.setEnvironmentProperties(configuration.illumination, configuration.humidity, configuration.temperature);
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


    std::vector<Plant*> plants(m_plant_storage.getPlants());
    /*
     * ITERATION # 1:
     * - Based on environmental factors calculates the strength of each plant
     * - Checks the status of the plant and acts accordingly (dies or grows)
     */
    for(Plant * p : plants)
    {
        // Shade
        int daily_illumination(m_environment_mgr.getDailyIllumination(p->m_center_position, p->getCanopyWidth(), p->getHeight()));

        // Humidity
        int soil_humidity(m_environment_mgr.getSoilHumidityPercentage(p->m_center_position, p->getRootSize(), p->m_unique_id));

        // Temperature [This is static but refetched each time in case it gets dynamic]
        int temp(m_environment_mgr.getTemperature(p->m_center_position));

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

            // Seeding
            if(m_simulation_options->per_plant_seeding_enabled && m_elapsed_months > 1 && m_elapsed_months % p->getSeedingInterval() == 0)
            {
                for(QPoint position : p->seed())
                    add_plant(m_plant_factory.generate(p->m_specie_id, position));
            }
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

    if((m_simulation_options->simplified_seeding_v1_enabled || m_simulation_options->simplified_seeding_v2_enabled)
            && m_elapsed_months > 1 && m_elapsed_months % 12 == 0)
    {
        PlantStorageStructure species(m_plant_storage.getSpecies());
        int total_plant_count(m_plant_storage.getPlantCount());

        for(auto specie(species.begin()); specie != species.end(); specie++)
        {
            if(specie->second.size() > 0) // Ignore if there are no species
            {
                int specie_id(specie->first);

                if(m_simulation_options->simplified_seeding_v1_enabled)
                {
                    int specie_seeds((((float)specie->second.size())/total_plant_count) * SIMPLIFIED_SEEDING_V1_SEED_COUNT);
                    for(int plant_count(0); plant_count < specie_seeds; plant_count++)
                        add_plant(m_plant_factory.generate(specie_id));
                }
                else
                {
                    std::vector<int> all_plant_ids;
                    for(auto plant(specie->second.begin()); plant != specie->second.end(); plant++)
                        all_plant_ids.push_back(plant->first);

                    const std::vector<int> all_plants_ids_backup(all_plant_ids);

                    for(int i = 0; i < SIMPLIFIED_SEEDING_V2_SEED_COUNT; i++)
                    {
                        if(all_plant_ids.size() == 0)
                            all_plant_ids = std::vector<int>(all_plants_ids_backup);

                        int random_index ( rand() % all_plant_ids.size() );
                        Plant * plant_to_seed(specie->second[all_plant_ids[random_index]]);
                        QPoint position (plant_to_seed->seed(1).at(0));
                        if(position.x() >= 0 && position.x() < AREA_WIDTH_HEIGHT &&
                                position.y() >= 0 && position.y() < AREA_WIDTH_HEIGHT)
                            add_plant(m_plant_factory.generate(specie_id, position));

                        all_plant_ids.erase(all_plant_ids.begin()+random_index);
                    }
                }
            }
        }
    }

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
