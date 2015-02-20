#include "environment_manager.h"

EnvironmentManager::EnvironmentManager() : m_spatial_hashmap(), m_resource_controllers(m_spatial_hashmap)
{

}

EnvironmentManager::~EnvironmentManager()
{
}

int EnvironmentManager::getShadedPercentage(QPoint p_center, float p_canopy_width, float p_height)
{
    return m_resource_controllers.illumination.getShadedPercentage(p_center, p_canopy_width, p_height);
}

int EnvironmentManager::getSoilHumidityPercentage(QPoint p_center, float p_roots_size, int p_id)
{
    return m_resource_controllers.soil_humidity.getHumidityPercentage(p_center, p_roots_size, p_id);
}

void EnvironmentManager::remove(QPoint p_center, float p_canopy_width, float p_roots_size, int p_id)
{
    m_resource_controllers.illumination.remove(p_center, p_canopy_width, p_id);
    m_resource_controllers.soil_humidity.remove(p_center, p_roots_size, p_id);
}

const EnvironmentSpatialHashMap & EnvironmentManager::getRenderingData()
{
    return m_spatial_hashmap;
}

void EnvironmentManager::reset()
{
    m_spatial_hashmap.clear();
}

void EnvironmentManager::updateEnvironment(QPoint p_center, float p_canopy_width, float p_height, float p_roots_size, int p_id, int p_minimum_soil_humidity_request)
{
    // Update illumination manager
    m_resource_controllers.illumination.setData(p_center, p_canopy_width, p_height, p_id);

    // Update soil humidity
    m_resource_controllers.soil_humidity.setData(p_center, p_roots_size, p_id, p_minimum_soil_humidity_request);
}

void EnvironmentManager::setEnvironmentProperties(const QImage & p_illumination_data, const QImage & p_soil_humidity_data)
{
    m_resource_controllers.illumination.setIlluminationData(p_illumination_data);
    m_resource_controllers.soil_humidity.setSoilHumidityData(p_soil_humidity_data);
}
