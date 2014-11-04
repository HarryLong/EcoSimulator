#include "plant.h"
#include "boost/foreach.hpp"
#include <array>
#include <set>
#include <algorithm>
#include "constants.h"

Plant::Plant(std::string p_name, QRgb p_color, Coordinate p_center_coord, long p_unique_id, int p_random_id, GrowthManager * p_growth_manager, AgeConstrainer * p_age_constrainer,
             IlluminationConstrainer * p_illumination_constrainer) :
    m_name(p_name), m_color(p_color), m_center_position(p_center_coord), m_unique_id(p_unique_id), m_random_id(p_random_id), m_growth_manager(p_growth_manager),
    m_age_constrainer(p_age_constrainer), m_current_canopy_radius(0), m_shadowed_points_ratio(0), m_overall_strength(1000),
    m_illumination_constrainer(p_illumination_constrainer)
{
    m_positions[m_center_position.x] = std::set<int>{m_center_position.y}; // Add the center coordinate to the positions
}

Plant::~Plant()
{
    delete m_growth_manager;
    delete m_age_constrainer;
}

void Plant::newMonth()
{
    m_age_constrainer->incrementAge();

    calculate_overall_strength();

    m_growth_manager->grow(1.0f); // TODO: Replace with calculated strength
    calculate_new_positions();
}

void Plant::calculate_overall_strength()
{
    // Age constrainer
    int age_strength(m_age_constrainer->getStrength());
    int illumination_strength(m_illumination_constrainer->getStrength(m_shadowed_points_ratio));

//    if(illumination_strength != 1000)
//        std::cout << "A " << m_name << " is partially covered! --> " << illumination_strength << std::endl;

//    std::cout << "Age strength: " << age_strength << " | " << " Illumination strength: " << illumination_strength << std::endl;

    m_overall_strength = min(age_strength,illumination_strength);
}

#define METER_IN_PIXELS 20
void Plant::calculate_new_positions()
{
    int new_canopy_radius_pixels = (m_growth_manager->getCanopyRadius()/100) * METER_IN_PIXELS;

    merge_coordinate_holders(m_positions, m_new_positions);
    m_new_positions.clear();

    if(new_canopy_radius_pixels != m_current_canopy_radius)
    {
        m_current_canopy_radius = new_canopy_radius_pixels;
        int previous_y(m_current_canopy_radius);
        int tmp(0);
        std::set<int> radius_set;
        for(int x = 0; x <= m_current_canopy_radius; x++)
        {
            int world_right_x(m_center_position.x + x);
            int world_left_x(m_center_position.x - x);
            if(world_right_x < AREA_WIDTH || world_left_x > 0)
            {
                int y(round(sqrt(pow(m_current_canopy_radius,2) - pow(x,2))));
                for(; previous_y >= y; previous_y--)
                {
                    // RIGHT SIDE OF CIRCLE
                    if((tmp = m_center_position.y + previous_y) < AREA_HEIGHT)
                        radius_set.insert(tmp);

                    if((tmp = m_center_position.y - previous_y) > 0)
                        radius_set.insert(tmp);
                }
                previous_y++; // Went one too far!

                std::vector<int> new_pos(radius_set.size());
                std::vector<int>::iterator endIt (std::set_difference(radius_set.begin(), radius_set.end(),
                                                                      m_positions[world_right_x].begin(), m_positions[world_right_x].end(), new_pos.begin()));
                new_pos.resize(endIt-new_pos.begin());

                if(world_right_x < AREA_WIDTH)
                {
                    m_new_positions[world_right_x].insert(new_pos.begin(), new_pos.end());
                    m_positions[world_right_x].insert(new_pos.begin(), new_pos.end());
                }
                if(world_left_x > 0)
                {
                    m_positions[world_left_x].insert(new_pos.begin(), new_pos.end());
                    m_new_positions[world_left_x].insert(new_pos.begin(), new_pos.end());
                }
            }
            radius_set.clear();
        }
    }
}

//void Plant::calculate_strength()
//{
//    float aggregated_probability_of_death(.0f);
//    for(int i = 0; i < m_constraint_count; i++)
//    {
//        aggregated_probability_of_death += m_constraints[i].process()
//    }


//    for(int i = 0; i < m_constraints; i++)

//    int importance_factor(1);
//    int max_possible_strength(0);

//    float strength(.0f);

//    /*******
//     * Age *
//     *******/
////    float age_strength(((float)m_species_properties->m_avg_max_age/m_plant_properties.age) / m_species_properties->m_avg_max_age);
////    age_strength *= importance_factor;
////    strength += age_strength;
////    max_possible_strength += importance_factor;

//    /****************
//     * Illumination *
//     ****************/
////    importance_factor = 2;
////    float illumination_strength(((float) m_plant_properties.positions.size() / max(m_plant_properties.shadowed_positions,1)) / m_plant_properties.positions.size());
////    illumination_strength *= importance_factor;
////    strength += illumination_strength;
////    max_possible_strength += importance_factor;

////    m_plant_properties.m_strength = (strength/max_possible_strength);

//    std::cout << "Strenght for " << m_species_properties->m_name << " : " << m_plant_properties.m_strength << std::endl;
//}
