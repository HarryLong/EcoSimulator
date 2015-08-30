#include "environment_manager.h"

EnvironmentManager::EnvironmentManager(int area_width, int area_height) :
    m_environment_spatial_hashmap(area_width, area_height), m_resource_controllers(m_environment_spatial_hashmap),
    m_month(-1)
{

}

EnvironmentManager::~EnvironmentManager()
{

}

int EnvironmentManager::getDailyIllumination(QPoint p_center, int p_id, float p_canopy_width, float height)
{
    return m_resource_controllers.illumination.getDailyIllumination(p_center, p_id, p_canopy_width, height);
}

int EnvironmentManager::getSoilHumidity(QPoint p_center, float p_roots_size, int p_id)
{
    return m_resource_controllers.soil_humidity.getSoilHumidity(p_center, p_roots_size, p_id);
}

int EnvironmentManager::getTemperature()
{
    return m_temperature.at(m_month-1);
}

void EnvironmentManager::remove(QPoint p_center, float p_canopy_width, float p_roots_size, int p_id)
{
    if(p_canopy_width > 0)
        m_resource_controllers.illumination.remove(p_center, p_canopy_width, p_id);
    m_resource_controllers.soil_humidity.remove(p_center, p_roots_size, p_id);
}

const EnvironmentSpatialHashMap & EnvironmentManager::getRenderingData()
{
    return m_environment_spatial_hashmap;
}

void EnvironmentManager::reset()
{
    m_environment_spatial_hashmap.clear();
}

void EnvironmentManager::updateEnvironment(QPoint p_center, float p_canopy_width, float p_height, float p_roots_size, int p_id, int p_minimum_soil_humidity_request)
{
    // Update illumination manager - No affect on illumination if canopy width is zero
    if(p_canopy_width > 0)
        m_resource_controllers.illumination.update(p_center, p_canopy_width, p_height, p_id);

    // Update soil humidity
    m_resource_controllers.soil_humidity.update(p_center, p_roots_size, p_id, p_minimum_soil_humidity_request);
}


void EnvironmentManager::setEnvironmentProperties( float slope, std::vector<int> humidity, std::vector<int> illumination, std::vector<int> temperature )
{
    m_environment_spatial_hashmap.resetAllCells();

    m_humidity = humidity;
    m_illumination = illumination;
    m_temperature = temperature;
}

void EnvironmentManager::setMonth(int p_month)
{
    m_month = p_month;
    m_environment_spatial_hashmap.setAvailableResources(m_illumination.at(p_month-1), m_humidity.at(p_month-1), m_temperature.at(p_month-1));
}

//void EnvironmentManager::setMonth(int p_month)
//{
//    m_resource_controllers.soil_humidity.refresh_resource_distribution(p_month);
//}

//std::pair<int,int> EnvironmentManager::getTemperatureRange()
//{
//    return m_resource_controllers.temperature.getRange();
//}

//std::pair<int,int> EnvironmentManager::getIlluminationRange()
//{
//    return m_resource_controllers.illumination.getRange();
//}

//std::pair<int,int> EnvironmentManager::getHumidityRange()
//{
//    return m_resource_controllers.soil_humidity.getRange();
//}
