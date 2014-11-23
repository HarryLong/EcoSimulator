#include "simulator_manager.h"

#include <iostream>
#include "plant.h"
#include <algorithm>
#include "boost/foreach.hpp"

#include "debuger.h"

static QRgb s_black_color_rgb(QColor(Qt::GlobalColor::black).rgb());

SimulatorManager::SimulatorManager() : m_time_keeper(), m_plant_storage(), m_plant_factory(), m_elapsed_months(0), m_state(Stopped),
    m_environment_mgr()
{
    m_time_keeper.addListener(this);
}

SimulatorManager::~SimulatorManager()
{
}

void SimulatorManager::add_plant(Plant * p_plant)
{
    m_plant_storage.add(p_plant);

    emit newPlant(QString(p_plant->m_name.c_str()), p_plant->m_color);
}

void SimulatorManager::remove_plant(int p_plant_id)
{
    m_plant_storage.remove(p_plant_id);
}

void SimulatorManager::start()
{
    m_state = Running;
    generate_random_plants();
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

    // TODO: First iterate once calculating strength based on previously generated environment data
    BOOST_FOREACH(Plant * p, m_plant_storage.getSortedPlants())
    {
        // TODO: Build list of potential intersecting plant ids to pass with it for calculating the strength
        float radius(p->getCanopyRadius());

        // Get
        float illumination (m_environment_mgr.getIlluminationPercentage(p->m_center_position, p->getCanopyRadius(), p->getHeight()));
        p->calculateStrength(illumination);
    }

    std::vector<int> plant_ids_to_remove;

    BOOST_FOREACH(Plant * p, m_plant_storage.getSortedPlants())
    {
        PlantStatus status (p->getStatus());
        if(status == Alive)
        {
            p->newMonth();

            // Update the environment
            m_environment_mgr.setSize(p->m_center_position, p->getCanopyRadius(), p->getHeight(), p->m_unique_id);

            // Insert the plant into list of plants to render
            m_plant_rendering_data.push_back(
                        PlantRenderingData(p->m_name, p->m_color, p->m_center_position, p->getHeight(), p->getCanopyRadius())
                        );
        }
        else
        {
            plant_ids_to_remove.push_back(p->m_unique_id);
            m_environment_mgr.remove(p->m_center_position, p->getCanopyRadius(), p->m_unique_id);
            emit removedPlant(QString(p->m_name.c_str()), plant_status_to_string(status));
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

PlantRenderDataContainer SimulatorManager::getPlantRenderingData()
{
    return m_plant_rendering_data;
}

IlluminationSpatialHashMap SimulatorManager::getIlluminationRenderingData()
{
    return m_environment_mgr.getIlluminationRenderingData();
}

void SimulatorManager::setMonthlyTriggerFrequency(int p_frequency)
{
    m_time_keeper.setUnitTime(p_frequency);
}

void SimulatorManager::generate_random_plants()
{
    add(Specie::OAK_TREE, 100);
    add(Specie::BANANA_TREE, 100);
    add(Specie::OLIVE_TREE, 100);
    add(Specie::PEANUT_TREE, 100);
}

void SimulatorManager::add(Specie specie, int count)
{
    for(int i = 0; i < count; i++)
        add_plant(m_plant_factory.generate(specie, generate_random_position()));
}

Coordinate SimulatorManager::generate_random_position()
{
    return Coordinate(rand()%AREA_WIDTH_HEIGHT, rand()%AREA_WIDTH_HEIGHT);
}

QString SimulatorManager::plant_status_to_string(PlantStatus status)
{
    switch (status){
        case DeathByAge:
            return QString("Age");
        case DeathByIllumination:
            return  QString("Light");
    default:
        return QString("This is a bug!");
    }
}

