#include "simulator_manager.h"

#include <iostream>
#include "plant.h"
#include <algorithm>
#include "boost/foreach.hpp"

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

void SimulatorManager::remove_plant(int p_plant_id)
{
    m_plant_storage.remove(p_plant_id);
}

void SimulatorManager::start( SimulationConfiguration configuration)
{
    m_environment_mgr.setEnvironmentProperties(configuration.illumination, configuration.soil_humidity);
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

    /*
     * ITERATION # 1:
     * - Based on environmental factors calculates the strength of each plant
     * - Checks the status of the plant and acts accordingly (dies or grows)
     */
    BOOST_FOREACH(Plant * p, m_plant_storage.getSortedPlants())
    {
        // Shade
        int daily_illumination(m_environment_mgr.getDailyIllumination(p->m_center_position, p->getCanopyWidth(), p->getHeight()));

        // Humidity
        int soil_humidity_percentage(m_environment_mgr.getSoilHumidityPercentage(p->m_center_position, p->getRootSize(), p->m_unique_id));

        p->calculateStrength(daily_illumination, soil_humidity_percentage);
    }

    // Check plant status
    std::vector<int> plant_ids_to_remove;

    /*
     * ITERATION # 1:
     * - Based on environmental factors calculates the strength of each plant
     * - Checks the status of the plant and acts accordingly (dies or grows)
     * - Updates the environment with the effect of the plant growth
     */
    BOOST_FOREACH(Plant * p, m_plant_storage.getSortedPlants())
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
            plant_ids_to_remove.push_back(p->m_unique_id);
            m_environment_mgr.remove(p->m_center_position, p->getCanopyWidth(), p->getRootSize(), p->m_unique_id);
            emit removedPlant(p->m_specie_name, plant_status_to_string(status));
        }
    }

    // Remove the plants
    BOOST_FOREACH(int id, plant_ids_to_remove)
    {
        remove_plant(id);
    }

    if(m_simulation_options->simplified_seeding_enabled && m_elapsed_months > 1 && m_elapsed_months % 12 == 0)
    {
        std::map<int,int> species (m_plant_storage.getSpecies());

        int total_plant_count(m_plant_storage.getPlantCount());

        for(auto it(species.begin()); it != species.end(); it++)
        {
            int specie_seeds((((float)it->second)/total_plant_count) * SIMPLIFIED_SEEDING_SEED_COUNT);
            for(int plant_count(0); plant_count < specie_seeds; plant_count++)
                add_plant(m_plant_factory.generate(it->first));
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
            return  QString("LightUnderExposure");
        case DeathByOverIllumination:
            return  QString("LightOverExposure");
        case DeathByFlood:
            return  QString("Flooded");
        case DeathByDrought:
            return  QString("Drought");
    default:
        return QString("This is a bug!");
    }
}

void SimulatorManager::refresh_rendering_data()
{
    m_plant_rendering_data.lock();
    m_plant_rendering_data.clear();

    BOOST_FOREACH(Plant * p, m_plant_storage.getSortedPlants())
    {
        m_plant_rendering_data.push_back(
                    PlantRenderingData(p->m_specie_name, p->getColor(), p->m_center_position, p->getHeight(), p->getCanopyWidth(), p->getRootSize()));
    }
    std::sort(m_plant_rendering_data.begin(), m_plant_rendering_data.end(),
              [](const PlantRenderingData & lhs, const PlantRenderingData & rhs) {return lhs.height < rhs.height;});

    m_plant_rendering_data.unlock();
}
