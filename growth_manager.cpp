#include "growth_manager.h"

GrowthManager::GrowthManager(std::shared_ptr<GrowthProperties> p_growth_properties) : m_growth_properties(p_growth_properties), m_height(.0f)
{

}

void GrowthManager::grow(int p_strength)
{
    m_height += (p_strength/1000.0f) * m_growth_properties->monthly_growth;
}
