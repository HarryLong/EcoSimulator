#include "growth_manager.h"
#include <math.h>

GrowthManager::GrowthManager(std::shared_ptr<GrowthProperties> p_growth_properties) :
    m_growth_properties(p_growth_properties), m_height(.0f), m_dice_roller(-5,5)
{

}

void GrowthManager::grow(int p_strength)
{
    float growth_percentage(std::min(1.0f, std::max(.0f, (p_strength + m_dice_roller.generate())/100.0f))); // In rage  [0,1]

    m_height += (growth_percentage * m_growth_properties->max_monthly_height_growth);
    m_root_radius += (growth_percentage * m_growth_properties->max_monthly_root_growth);
}
