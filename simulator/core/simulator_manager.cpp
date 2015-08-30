#include "simulator_manager.h"

#include <iostream>
#include <algorithm>
#include <mutex>

#include "../../utils/utils.h"
#include "../plants/plant.h"

#include <QDebug>

static QRgb s_black_color_rgb(QColor(Qt::GlobalColor::black).rgb());

SimulatorManager::SimulatorManager() : m_time_keeper(),
    m_plant_storage(SimulatorManager::_AREA_WIDTH_HEIGHT, SimulatorManager::_AREA_WIDTH_HEIGHT),
    m_environment_mgr(SimulatorManager::_AREA_WIDTH_HEIGHT, SimulatorManager::_AREA_WIDTH_HEIGHT),
    m_plant_factory(SimulatorManager::_AREA_WIDTH_HEIGHT, SimulatorManager::_AREA_WIDTH_HEIGHT),
    m_elapsed_months(0), m_state(Stopped), m_plant_rendering_data()
{
    m_time_keeper.addListener(this);
}

SimulatorManager::~SimulatorManager()
{

}

void SimulatorManager::add_plant(Plant * p_plant)
{
    if(!m_plant_storage.isPlantAtLocation(p_plant->m_center_position))
    {
        m_plant_storage.add(p_plant);
        m_environment_mgr.updateEnvironment(p_plant->m_center_position, p_plant->getCanopyWidth(), p_plant->getHeight(), p_plant->getRootSize(),
                                            p_plant->m_unique_id, p_plant->getMinimumSoilHumidityRequirement()); // Update resources in environment

        emit newPlant(p_plant->m_specie_name, p_plant->getColor());
    }
}

void SimulatorManager::remove_plant(Plant * p)
{
    m_plant_storage.remove(p);
}

void SimulatorManager::start( SimulationConfiguration configuration)
{
    m_environment_mgr.setEnvironmentProperties(configuration.m_slope,
                                               configuration.m_humidity,
                                               configuration.m_illumination,
                                               configuration.m_temperature);

    m_seeding_enabled = configuration.m_seeding_enabled;
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

//    m_environment_mgr.reset();
    m_plant_rendering_data.lock();
    m_plant_rendering_data.clear();
    m_plant_rendering_data.unlock();

    m_plant_storage.clear();
    m_elapsed_months = 0;

    emit updated(1);// In order to remove on screen elements
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

#pragma omp parallel for
    for(int i = 0; i < plants.size(); i++)
    {
        Plant * p = plants.at(i);
        // Shade
        int daily_illumination(m_environment_mgr.getDailyIllumination(p->m_center_position, p->m_unique_id, p->getCanopyWidth(), p->getHeight()));

        // Humidity
        int soil_humidity(m_environment_mgr.getSoilHumidity(p->m_center_position, p->getRootSize(), p->m_unique_id));

        // Temperature [This is static but refetched each time in case it gets dynamic]
        int temp(m_environment_mgr.getTemperature());

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
    for(int i = 0; i < plants.size(); i++)
    {
        Plant * p = plants.at(i);
        Plant::PlantStatus status (p->getStatus());
        if(status == Plant::PlantStatus::Alive)
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
    if(m_seeding_enabled && m_elapsed_months % 12 == 6)
    {
        PlantStorageStructure species(m_plant_storage.getPlantsBySpecies());

        for(auto specie(species.begin()); specie != species.end(); specie++)
        {
            int specie_id(specie->first);
            int specie_seed_count(m_plant_factory.getSpecieProperties(specie_id)->seeding_properties->seed_count);

            if(specie->second.size() > 0) // Use existing plants to seed
            {
                std::vector<Plant*> seeding_plants(m_plant_storage.getOnePlantPerCell(specie_id));

                qCritical() << "Plant of specie: " << specie->second.size();
                qCritical() << "Seeding plants of specie count: " << seeding_plants.size();

                auto plant_it(seeding_plants.begin());

                int seed_count(0);
                while(seed_count++ < specie_seed_count)
                {
                    Plant * seeding_plant (*plant_it);
                    QPoint position (seeding_plant->seed(1).at(0));
                    if(position.x() >= 0 && position.x() < SimulatorManager::_AREA_WIDTH_HEIGHT &&
                        position.y() >= 0 && position.y() < SimulatorManager::_AREA_WIDTH_HEIGHT)
                    {
                        add_plant(m_plant_factory.generate(specie_id, position));
                    }

                    if(++plant_it == seeding_plants.end())
                        plant_it = seeding_plants.begin();
                }
            }
            else // spawn new plants
            {
                int n_planted(0);
                const SpecieProperties * specie_properties (m_plant_factory.getSpecieProperties(specie_id));
                if(specie_properties->illumination_properties->min_illumination == 0 && m_elapsed_months > 240)
                {
                    // shade loving - spawn half at existing plant locations (shaded)
                    std::vector<Plant*> plants (m_plant_storage.getPlants());
                    for(; n_planted < specie_seed_count/2; n_planted++)
                    {
                        // Select a random plant
                        Plant * random_plant(*(plants.begin() + (rand()%plants.size())));
                        QPoint location(Utils::getRandomPointInCircle(random_plant->m_center_position,
                                                                            std::max(1.0f,random_plant->getCanopyWidth()/2.f)));
                        add_plant(m_plant_factory.generate(specie_id, location));
                    }
                }
                for(; n_planted < specie_seed_count; n_planted++)
                    add_plant(m_plant_factory.generate(specie_id));
            }
        }
    }

    m_plant_storage.unlock();
    refresh_rendering_data();

    emit updated(month);
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

QString SimulatorManager::plant_status_to_string(Plant::PlantStatus status)
{
    switch (status){
        case Plant::PlantStatus::DeathByAge:
            return QString("Age");
        case Plant::PlantStatus::DeathByUnderIllumination:
            return  QString("Insufficient sunlight");
        case Plant::PlantStatus::DeathByOverIllumination:
            return  QString("Too much sunlight");
        case Plant::PlantStatus::DeathByFlood:
            return  QString("Flooded");
        case Plant::PlantStatus::DeathByDrought:
            return  QString("Drought");
        case Plant::PlantStatus::DeathByCold:
            return QString("Cold");
        case Plant::PlantStatus::DeathByHeat:
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
//    new std::thread(&PlantStorage::generateStatisticalSnapshot, &m_plant_storage, m_environment_mgr.getHumidityRange(), m_environment_mgr.getIlluminationRange(),
//                    m_environment_mgr.getTemperatureRange(), m_elapsed_months);
}
