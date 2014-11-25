#include "environment_manager.h"

EnvironmentManager::EnvironmentManager()
{

}

int EnvironmentManager::getShadedPercentage(QPoint p_center, float p_canopy_radius, float p_height)
{
    return m_factors.illumination.getShadedPercentage(p_center, p_canopy_radius, p_height);
}

int EnvironmentManager::getSoilHumidityPercentage(QPoint p_center, float p_roots_radius, int p_id)
{
    return m_factors.soil_humidity.getHumidityPercentage(p_center, p_roots_radius, p_id);
}

void EnvironmentManager::remove(QPoint p_center, float p_radius, int id)
{
    m_factors.illumination.remove(p_center, p_radius, id);
}

IlluminationSpatialHashMap EnvironmentManager::getIlluminationRenderingData()
{
    return m_factors.illumination.getIlluminationSpatialHashmap();
}

SoilHumiditySpatialHashMap EnvironmentManager::getSoilHumidityRenderingData()
{
    return m_factors.soil_humidity.getSoilHumiditySpatialHashmap();
}

void EnvironmentManager::reset()
{
    m_factors.illumination.reset();
    m_factors.soil_humidity.reset();
}

void EnvironmentManager::updateEnvironment(QPoint p_center, float p_canopy_radius, float p_height, float p_root_radius, int p_id, int p_minimum_soil_humidity_request)
{
    // Update illumination manager
    m_factors.illumination.setData(p_center, p_canopy_radius, p_height, p_id);

    // Update soil humidity
    m_factors.soil_humidity.setData(p_center, p_root_radius, p_id, p_minimum_soil_humidity_request);
}
