#include "growth_manager.h"
#include <math.h>
#include <iostream>

GrowthManager::GrowthManager(const GrowthProperties * p_growth_properties, const AgeingProperties* p_ageing_properties) :
    m_max_monthly_canopy_growth(p_growth_properties->max_canopy_width/ (12 * p_ageing_properties->start_of_decline)),
    m_max_monthly_height_growth(p_growth_properties->max_height/ (12 * p_ageing_properties->start_of_decline)),
    m_max_monthly_root_growth(p_growth_properties->max_root_size/(12 * p_ageing_properties->start_of_decline)),
    m_height(1.0f), m_root_size(1.0f), m_canopy_width(.0f), m_dice_roller(-5,5)
{
}

void GrowthManager::grow(int p_strength)
{
    float growth_percentage(std::min(1.0f, std::max(.0f, (p_strength + m_dice_roller.generate())/100.0f))); // In rage  [0,1]

    m_height += (growth_percentage * m_max_monthly_height_growth);
    m_root_size += (growth_percentage * m_max_monthly_root_growth);
    m_canopy_width += (growth_percentage * m_max_monthly_canopy_growth);
}

float GrowthManager::getHeight() const
{
    return m_height;
}

float GrowthManager::getCanopyWidth() const
{
    return m_canopy_width;
}

float GrowthManager::getRootSize() const
{
    return m_root_size;
}
