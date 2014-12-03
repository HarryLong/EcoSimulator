#include "simulator_manager.h"

#include <iostream>
#include "plant.h"
#include <algorithm>
#include "boost/foreach.hpp"

#include "debuger.h"

static QRgb s_black_color_rgb(QColor(Qt::GlobalColor::black).rgb());

SimulatorManager::SimulatorManager() : m_time_keeper(), m_plant_storage(), m_elapsed_months(0), m_state(Stopped),
    m_environment_mgr()
{
    m_time_keeper.addListener(this);
}

SimulatorManager::~SimulatorManager()
{
}

void SimulatorManager::addPlants(std::vector<Plant *> p_plants)
{
    for(Plant * p : p_plants)
        addPlant(p);
}

void SimulatorManager::addPlant(Plant * p_plant)
{
    m_plant_storage.add(p_plant);
    m_environment_mgr.updateEnvironment(p_plant->m_center_position, p_plant->getCanopyRadius(), p_plant->getHeight(), p_plant->getRootsRadius(),
                                        p_plant->m_unique_id, p_plant->getMinimumSoilHumidityRequirement()); // Update resources in environment

    emit newPlant(p_plant->m_name, p_plant->m_color);
}

void SimulatorManager::remove_plant(int p_plant_id)
{
    m_plant_storage.remove(p_plant_id);
}

void SimulatorManager::start()
{
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

    m_environment_mgr.reset();
    m_plant_rendering_data.clear();
    m_plant_storage.clear();
    m_elapsed_months = 0;

    emit update();// In order to remove on screen elements
}

void SimulatorManager::newMonth()
{
    if(m_stopping.load())
        return;

    m_elapsed_months++;

    m_plant_rendering_data.clear();

    /*
     * ITERATION # 1:
     * - Based on environmental factors calculates the strength of each plant
     * - Checks the status of the plant and acts accordingly (dies or grows)
     */
    BOOST_FOREACH(Plant * p, m_plant_storage.getSortedPlants())
    {
        // Shade
        int shaded_percentage (m_environment_mgr.getShadedPercentage(p->m_center_position, p->getCanopyRadius(), p->getHeight()));
        // Humidity
        int soil_humidity_percentage(m_environment_mgr.getSoilHumidityPercentage(p->m_center_position, p->getRootsRadius(), p->m_unique_id));

        p->calculateStrength(shaded_percentage, soil_humidity_percentage);
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
            m_environment_mgr.updateEnvironment(p->m_center_position, p->getCanopyRadius(), p->getHeight(),
                                                p->getRootsRadius(), p->m_unique_id, p->getMinimumSoilHumidityRequirement());

            // Insert the plant into list of plants to render
            m_plant_rendering_data.push_back(
                        PlantRenderingData(p->m_name, p->m_color, p->m_center_position, p->getHeight(), p->getCanopyRadius(), p->getRootsRadius()));
        }
        else
        {
            plant_ids_to_remove.push_back(p->m_unique_id);
            m_environment_mgr.remove(p->m_center_position, p->getCanopyRadius(), p->getRootsRadius(), p->m_unique_id);
            emit removedPlant(p->m_name, plant_status_to_string(status));
        }
    }

    // Remove the plants
    BOOST_FOREACH(int id, plant_ids_to_remove)
    {
        remove_plant(id);
    }

    std::sort(m_plant_rendering_data.begin(), m_plant_rendering_data.end(),
              [](const PlantRenderingData & lhs, const PlantRenderingData & rhs) {return lhs.height < rhs.height;});

    emit update();
}

void SimulatorManager::setIllumination(const QImage* p_illumination_data)
{
    m_environment_mgr.setIllumination(p_illumination_data);
}

void SimulatorManager::setSoilHumidity(const QImage* p_soil_humidity_data)
{
    m_environment_mgr.setSoilHumidity(p_soil_humidity_data);
}

PlantRenderDataContainer SimulatorManager::getPlantRenderingData()
{
    return m_plant_rendering_data;
}

IlluminationSpatialHashMap SimulatorManager::getIlluminationRenderingData()
{
    return m_environment_mgr.getIlluminationRenderingData();
}

SoilHumiditySpatialHashMap SimulatorManager::getSoilHumidityRenderingData()
{
    return m_environment_mgr.getSoilHumidityRenderingData();
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
        case DeathByIllumination:
            return  QString("Light");
        case DeathByFlood:
            return  QString("Flooded");
        case DeathByDrought:
            return  QString("Drought");
    default:
        return QString("This is a bug!");
    }
}

