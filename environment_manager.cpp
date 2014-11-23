#include "environment_manager.h"

EnvironmentManager::EnvironmentManager()
{

}

float EnvironmentManager::getIlluminationPercentage(Coordinate p_center, float p_radius, float p_height)
{
    return m_factors.illumination.getIlluminationPercentage(p_center, p_radius, p_height);
}

void EnvironmentManager::setSize(Coordinate p_center, float p_radius, float p_height, int id)
{
    m_factors.illumination.setSize(p_center, p_radius, p_height, id);
}

void EnvironmentManager::remove(Coordinate p_center, float p_radius, int id)
{
    m_factors.illumination.remove(p_center, p_radius, id);
}

IlluminationSpatialHashMap EnvironmentManager::getIlluminationRenderingData()
{
    return m_factors.illumination.getIlluminationSpatialHashmap();
}

void EnvironmentManager::reset()
{
    m_factors.illumination.reset(); // Reset illumination
}
