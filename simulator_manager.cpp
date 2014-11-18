#include "simulator_manager.h"

#include <iostream>
#include "plant.h"
#include <algorithm>
#include "boost/foreach.hpp"

static QRgb s_black_color_rgb(QColor(Qt::GlobalColor::black).rgb());

SimulatorManager::SimulatorManager() : m_time_keeper(), m_plant_storage(), m_plant_factory(), m_world_plant_id_ref(AREA_WIDTH*AREA_HEIGHT),
    m_drawn_radii(), m_elapsed_months(0)
{
    std::generate(m_world_plant_id_ref.begin(), m_world_plant_id_ref.end(), []() { return new std::unordered_set<int>();});

    m_time_keeper.addListener(this);
    generate_random_plants();
}

SimulatorManager::~SimulatorManager()
{
    for(int i = 0; i < AREA_WIDTH*AREA_HEIGHT; i++)
    {
        delete m_world_plant_id_ref[i];
    }
}

void SimulatorManager::addPlant(Plant * p_plant)
{
    m_plant_storage.add(p_plant);
    int index = to_index(p_plant->m_center_position);
    p_plant->addToWorldIndices(index);
    m_world_plant_id_ref[index]->insert(p_plant->m_unique_id);
    m_drawn_radii[p_plant->m_unique_id] = 0;
}

void SimulatorManager::remove_plant_data(Plant * p_plant)
{
    m_drawn_radii.erase(p_plant->m_unique_id);
    BOOST_FOREACH(int index, p_plant->getWorldIndices())
    {
        m_world_plant_id_ref[index]->erase(p_plant->m_unique_id);
    }

    // Remove plants which reference it as an intersecting plant
    BOOST_FOREACH(int intersecting_plant_id, p_plant->getIntersectingPlantIds())
    {
        m_plant_storage.removeIntersectingPlantId(intersecting_plant_id, p_plant->m_unique_id);
    }
}

void SimulatorManager::start()
{
    m_time_keeper.start();
}

void SimulatorManager::stop()
{
    m_time_keeper.stop();
}

void SimulatorManager::trigger()
{
    m_elapsed_months++;

    std::vector<int> plant_ids_to_remove;
    std::set<int> intersecting_plant_ids;
    m_plant_rendering_data.clear();
    BOOST_FOREACH(Plant * p, m_plant_storage.getSortedPlants())
    {
        p->calculateStrength(m_plant_storage);
        intersecting_plant_ids.clear();
        if(p->survives())
        {
            p->newMonth();
            int old_canopy_radius(m_drawn_radii[p->m_unique_id]);
            int new_canopy_radius(METER_IN_PIXELS* (p->getCanopyRadius()/100));
            if(new_canopy_radius - old_canopy_radius > 0)
            {
                int center_x(p->m_center_position.x);
                int center_y(p->m_center_position.y);
                for(int i = -new_canopy_radius; i <= new_canopy_radius; i++)
                {
                    for(int k = old_canopy_radius+1; k <= new_canopy_radius; k++)
                    {
                        std::vector<Coordinate> coordinates_to_add;
                        // TOP
                        coordinates_to_add.push_back(Coordinate(center_x + i, center_y + k));
                        // BOTTOM
                        coordinates_to_add.push_back(Coordinate(center_x + i, center_y - k));

                        if(i >= - old_canopy_radius && i <= old_canopy_radius)
                        {
                            // LEFT
                            coordinates_to_add.push_back(Coordinate(center_x - k, center_y + i));
                            // RIGHT
                            coordinates_to_add.push_back(Coordinate(center_x + k, center_y + i));
                        }

                        BOOST_FOREACH(Coordinate c, coordinates_to_add)
                        {
                            if(c.x < AREA_WIDTH && c.x >= 0 &&
                                    c.y < AREA_HEIGHT && c.y > 0)
                            {
                                int index(to_index(c));
                                p->addToWorldIndices(index);

                                // Manage the intersecting plants
                                m_plant_storage.addIntersectingPlantIds(p->m_unique_id, m_world_plant_id_ref[index]);
                                // Add to the world map
                                m_world_plant_id_ref[index]->insert(p->m_unique_id);
                            }
                        }
                    }
                }
                m_drawn_radii[p->m_unique_id] = new_canopy_radius;
            }
            // Insert the plant into list of plants to render
            m_plant_rendering_data.push_back(
                        PlantRenderingData(p->m_name, p->m_color, p->m_center_position, p->getHeight(), p->getCanopyRadius())
                        );
        }
        else
        {
            std::cout << "REMOVING PLANT" << std::endl;
            plant_ids_to_remove.push_back(p->m_unique_id);
            remove_plant_data(p);
        }
    }
    // Remove the plants
    BOOST_FOREACH(int id, plant_ids_to_remove)
    {
        m_plant_storage.remove(id);
    }

    std::sort(m_plant_rendering_data.begin(), m_plant_rendering_data.end(),
              [](const PlantRenderingData & lhs, const PlantRenderingData & rhs) {return lhs.height < rhs.height;});

    emit update();
}

PlantRenderDataContainer SimulatorManager::getPlantData()
{
    return m_plant_rendering_data;
}

PlantBoundingBoxContainer SimulatorManager::getBoundingBoxData()
{
    return m_world_plant_id_ref;
}

void SimulatorManager::setMonthlyTriggerFrequency(int p_frequency)
{
    m_time_keeper.setUnitTime(p_frequency);

}

Coordinate SimulatorManager::to_coord(int p_index)
{
    return Coordinate(p_index%AREA_WIDTH, (p_index/AREA_WIDTH));;
}

int SimulatorManager::to_index(Coordinate p_coord)
{
    return ((p_coord.y * AREA_WIDTH) + p_coord.x);
}

void SimulatorManager::generate_random_plants()
{
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));

    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, to_coord(get_random_index())));

    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, to_coord(get_random_index())));

    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, to_coord(get_random_index())));

    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));

    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, to_coord(get_random_index())));

    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, to_coord(get_random_index())));

    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, to_coord(get_random_index())));

    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, to_coord(get_random_index())));

//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));

//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));

//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));

//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));

//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));

//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));

//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));

//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));

//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));

//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));

//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));

//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));

//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));

//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));

//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
//    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
}

int SimulatorManager::get_random_index()
{
    int random_position = rand()%(AREA_WIDTH*AREA_HEIGHT);    
    while(!m_world_plant_id_ref[random_position]->empty())
        random_position = rand()%(AREA_WIDTH*AREA_HEIGHT);
    return random_position;
}
