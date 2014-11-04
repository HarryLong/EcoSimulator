#include "growth_manager.h"

GrowthManager::GrowthManager(const GrowthProperties * p_growth_properties) : m_growth_properties(p_growth_properties), m_height(.0f)
{

}

void GrowthManager::grow(float p_strength)
{
    m_height +=  p_strength * m_growth_properties->monthly_growth;
}
