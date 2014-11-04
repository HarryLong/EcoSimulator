#include "simulator_manager.h"
#include <iostream>
#include <stdlib.h>
#include "plant.h"

#include "boost/foreach.hpp"

static QRgb s_black_color_rgb(QColor(Qt::GlobalColor::black).rgb());

SimulatorManager::SimulatorManager() : m_time_keeper(), m_display(new DisplayArea(AREA_WIDTH, AREA_HEIGHT)),
    m_plants(), m_plant_factory(), m_world_plant_ref{std::vector<Plant*>()}
{
    m_time_keeper.addListener(this);

    m_display_data = (QRgb*) calloc(AREA_WIDTH * AREA_HEIGHT, sizeof(QRgb));

    // TEMP
    generate_random_plants();
}

SimulatorManager::~SimulatorManager()
{
    delete[] m_display;
    typedef std::map<long, Plant*> map_type;
    BOOST_FOREACH(map_type::value_type pair, m_plants)
    {
        delete pair.second;
    }
}

void SimulatorManager::addPlant(Plant * p_plant)
{
    int index = toIndex(p_plant->m_center_position);
    m_plants[p_plant->m_unique_id] = p_plant;
    m_world_plant_ref[index].push_back(p_plant);
    m_display_data[index] = p_plant->m_color;
}

void SimulatorManager::remove_plant_data(Plant * p_plant)
{
    // Remove all referenced positions
    int id(p_plant->m_unique_id);
    std::cout << "Removing : " << p_plant->m_name << " [ " << id << " ] " << std::endl;
    BOOST_FOREACH(auto coordinate_holder, p_plant->getPositions())
    {
        int base_x = coordinate_holder.first;
        BOOST_FOREACH(int y, coordinate_holder.second)
        {
            Coordinate coord(base_x,y);
            int index(toIndex(coord));
            m_world_plant_ref[index].erase(std::find(m_world_plant_ref[index].begin(), m_world_plant_ref[index].end(),p_plant));
            if(!is_shadowed(p_plant, coord))
            {
                m_display_data[index] = s_black_color_rgb;
            }
        }
    }
}

void SimulatorManager::start()
{
    m_time_keeper.start();
}

static int age(0);
void SimulatorManager::trigger()
{
    std::cout << "--Age: " << ++age << std::endl;
    std::vector<int> plant_ids_to_remove;
    for(plant_map_type::iterator this_plant_it(m_plants.begin()); this_plant_it != m_plants.end(); ++this_plant_it)
    {
        Plant * p  = this_plant_it->second;

        if(p->survives())
        {
            p->newMonth();

            QRgb plant_color(p->m_color);
            BOOST_FOREACH(auto & coordinate_holder, p->getNewPositions())
            {
                int base_x = coordinate_holder.first;
                BOOST_FOREACH(int y, coordinate_holder.second)
                {
                    Coordinate coord(base_x, y);
                    int index(toIndex(coord));
                    m_world_plant_ref[index].push_back(p);
                }
            }
            // SHADOW + COLOR CALCULATIONS
            int shadowed_points(0);
            int total_points(0);
            BOOST_FOREACH(auto & coordinate_holder, p->getPositions())
            {
                int base_x = coordinate_holder.first;
                BOOST_FOREACH(int y, coordinate_holder.second)
                {
                    total_points++;
                    Coordinate coord(base_x, y);
                    int index(toIndex(coord));

                    if(is_shadowed(p, coord))
                    {
                        shadowed_points++;
                    }
                    else
                    {
                        m_display_data[index] = plant_color;
                    }
                }
            }
            p->setShadowedPointsRatio(((float) shadowed_points)/total_points);
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
        delete m_plants[id];
        m_plants.erase(id);
    }

    m_display->setData((uchar*) m_display_data);
}

void SimulatorManager::setTimeAcceleration(int p_acceleration)
{
    m_time_keeper.setUnitTime((MAX_UNIT_TIME+1)-p_acceleration);
}

bool SimulatorManager::check_position_valid(Coordinate c)
{
    return c.x < AREA_WIDTH && c.y < AREA_HEIGHT;
}

bool SimulatorManager::is_shadowed(const Plant * p_plant, const Coordinate & p_coord)
{
    BOOST_FOREACH(Plant * p, m_world_plant_ref[toIndex(p_coord)])
    {
        if( p_plant != p && p_plant->getHeight() < p->getHeight())
        {
            return true;
        }
    }
    return false;
}

void SimulatorManager::generate_random_plants()
{
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));

    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));

    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));

    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::BANANA_TREE, toCoord(get_random_index())));

    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OAK_TREE, toCoord(get_random_index())));

    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::OLIVE_TREE, toCoord(get_random_index())));

    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));
    addPlant(m_plant_factory.generate(Specie::PEANUT_TREE, toCoord(get_random_index())));

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

Coordinate SimulatorManager::toCoord(int p_index)
{
    return Coordinate(p_index%AREA_WIDTH, (p_index/AREA_WIDTH));;
}

int SimulatorManager::toIndex(Coordinate p_coord)
{
    return ((p_coord.y * AREA_WIDTH) + p_coord.x);
}

int SimulatorManager::get_random_index()
{
    int random_position = rand()%(AREA_WIDTH*AREA_HEIGHT);
    while(!m_world_plant_ref[random_position].empty())
        random_position = rand()%(AREA_WIDTH*AREA_HEIGHT);
    return random_position;
}
