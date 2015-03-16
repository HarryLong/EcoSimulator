#include "environment_manager.h"

EnvironmentManager::EnvironmentManager() : m_spatial_hashmap(), m_resource_controllers(m_spatial_hashmap)
{

}

EnvironmentManager::~EnvironmentManager()
{
}

int EnvironmentManager::getDailyIllumination(int p_month, QPoint p_center, float p_canopy_width, float p_height)
{
    return m_resource_controllers.illumination.getDailyIllumination(p_month, p_center, p_canopy_width, p_height);
}

int EnvironmentManager::getSoilHumidityPercentage(QPoint p_center, float p_roots_size, int p_id)
{
    return m_resource_controllers.soil_humidity.getHumidityPercentage(p_center, p_roots_size, p_id);
}

int EnvironmentManager::getTemperature(int p_month, QPoint p_center)
{
    return m_resource_controllers.temperature.getTemperature(p_month, p_center);
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

void EnvironmentManager::setEnvironmentProperties(PixelData * p_illumination_data, int p_illumination_variance,
                                                  PixelData * p_soil_humidity_data, int p_humidity_variance,
                                                  PixelData * p_temperature_data, int p_temperature_variance)
{
    m_resource_controllers.illumination.setIlluminationData(p_illumination_data, p_illumination_variance);
    m_resource_controllers.soil_humidity.setSoilHumidityData(p_soil_humidity_data, p_humidity_variance);
    m_resource_controllers.temperature.setTemperatureData(p_temperature_data, p_temperature_variance);
}

void EnvironmentManager::setMonth(int p_month)
{
//    for(int x(0); x < m_spatial_hashmap.getHorizontalCellCount(); x++)
//        for(int y(0); y < m_spatial_hashmap.getVerticalCellCount(); y++)
//            m_spatial_hashmap.get(QPoint(x,y))->setMonth(p_month);

    m_resource_controllers.soil_humidity.refresh_resource_distribution(p_month);
}
