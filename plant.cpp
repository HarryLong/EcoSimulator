#include "plant.h"
#include "boost/foreach.hpp"

#include "illumination_constrainer.h"
#include "age_constrainer.h"
#include "growth_manager.h"

#include "constants.h"

Plant::Plant(std::string p_name, QColor p_color, Coordinate p_center_coord, long p_unique_id, int p_random_id, GrowthManager * p_growth_manager, AgeConstrainer * p_age_constrainer,
             IlluminationConstrainer * p_illumination_constrainer) :
    m_name(p_name), m_color(p_color), m_center_position(p_center_coord), m_unique_id(p_unique_id), m_random_id(p_random_id), m_growth_manager(p_growth_manager),
    m_age_constrainer(p_age_constrainer), m_illumination_constrainer(p_illumination_constrainer), m_intersecting_plant_ids(), m_world_indices(), m_strengths()
{
}

Plant::~Plant()
{
    delete m_growth_manager;
    delete m_age_constrainer;
}

void Plant::newMonth()
{
    m_age_constrainer->incrementAge();

    m_growth_manager->grow(m_strengths.getMin()); // TODO: Replace with calculated strength
}

void Plant::addToWorldIndices(int index)
{
    m_world_indices.push_back(index);
}

void Plant::calculateStrength(const PlantStorage & p_plant_storage)
{
    /*******
     * Age *
     *******/
    m_strengths.age = m_age_constrainer->getStrength();

    /****************
     * Illumination *
     ****************/
    float total_intersection_area(.0f);
    BOOST_FOREACH(int intersecting_plant_id, m_intersecting_plant_ids)
    {
        const Plant * p (p_plant_storage.get(intersecting_plant_id));
        if(p->getHeight() >= getHeight())
            total_intersection_area += calculate_intersection_area(p->m_center_position, p->getCanopyRadius());
    }
//    float total_area(PI * pow(max(getCanopyRadius(),.1f), 2));

    m_strengths.illumination = m_illumination_constrainer->getStrength(total_intersection_area/(PI * pow(max(getCanopyRadius(),.1f), 2)));

    /*if(total_intersection_area > 0)
    {
        std::cout << "Total intersectio area: " << total_intersection_area << std::endl;
        std::cout << "Age strength: " << age_strength << " | Illuminsation strength: " << illumination_strength << std::endl;
    }*/
}

float Plant::calculate_intersection_area(const Coordinate & p_circle_center, float p_circle_radius)
{
    // get the largest radius
    float largest_radius( getCanopyRadius() > p_circle_radius ? getCanopyRadius() : p_circle_radius );

    // First calculate distance d between both centers
    int d_pixels ( sqrt( pow((p_circle_center.x-m_center_position.x), 2) + pow((p_circle_center.y-m_center_position.y), 2) ) );
    float d_cm ( ((float)d_pixels / METER_IN_PIXELS) *100.f );

    // Now calculate the overlap
    float overlap ( -1 * (d_cm - getCanopyRadius() - p_circle_radius) );

    if(overlap < 0) // No overlap
        return .0f;

    // Calculate the x coordinate of the center of the overlap
    float center_x ( largest_radius - (overlap / 2 ));

    // Calculate the y coordinate of the intersection
    float y ( sqrt(pow(largest_radius,2) - pow(center_x, 2)) );

    float intersection_area ( overlap * y );

    return intersection_area;
}

void Plant::calculate_new_positions()
{
//    int new_canopy_radius_pixels = (m_growth_manager->getCanopyRadius()/100) * METER_IN_PIXELS;

//    merge_coordinate_holders(m_positions, m_new_positions);
//    m_new_positions.clear();

//    if(new_canopy_radius_pixels != m_current_canopy_radius)
//    {
//        m_current_canopy_radius = new_canopy_radius_pixels;
//        int previous_y(m_current_canopy_radius);
//        int tmp(0);
//        std::set<int> radius_set;
//        for(int x = 0; x <= m_current_canopy_radius; x++)
//        {
//            int world_right_x(m_center_position.x + x);
//            int world_left_x(m_center_position.x - x);
//            if(world_right_x < AREA_WIDTH || world_left_x > 0)
//            {
//                int y(round(sqrt(pow(m_current_canopy_radius,2) - pow(x,2))));
//                for(; previous_y >= y; previous_y--)
//                {
//                    // RIGHT SIDE OF CIRCLE
//                    if((tmp = m_center_position.y + previous_y) < AREA_HEIGHT)
//                        radius_set.insert(tmp);

//                    if((tmp = m_center_position.y - previous_y) > 0)
//                        radius_set.insert(tmp);
//                }
//                previous_y++; // Went one too far!

//                std::vector<int> new_pos(radius_set.size());
//                std::vector<int>::iterator endIt (std::set_difference(radius_set.begin(), radius_set.end(),
//                                                                      m_positions[world_right_x].begin(), m_positions[world_right_x].end(), new_pos.begin()));
//                new_pos.resize(endIt-new_pos.begin());

//                if(world_right_x < AREA_WIDTH)
//                {
//                    m_new_positions[world_right_x].insert(new_pos.begin(), new_pos.end());
//                    m_positions[world_right_x].insert(new_pos.begin(), new_pos.end());
//                }
//                if(world_left_x > 0)
//                {
//                    m_positions[world_left_x].insert(new_pos.begin(), new_pos.end());
//                    m_new_positions[world_left_x].insert(new_pos.begin(), new_pos.end());
//                }
//            }
//            radius_set.clear();
//        }
//    }
}

void Plant::addIntersectingPlantId(int p_plant_id)
{
    if(previously_appended_interesecting_plant_id != p_plant_id)
    {
        m_intersecting_plant_ids.insert(p_plant_id);
        previously_appended_interesecting_plant_id = p_plant_id;
    }
}

void Plant::removeIntersectingPlantId(int p_plant_id)
{
    m_intersecting_plant_ids.erase(p_plant_id);
}


float Plant::getHeight() const
{
    return m_growth_manager->getHeight();
}

float Plant::getCanopyRadius() const
{
    return m_growth_manager->getCanopyRadius();
}
