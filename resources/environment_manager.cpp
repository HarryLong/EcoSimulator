#include "environment_manager.h"

EnvironmentManager::EnvironmentManager(int area_width, int area_height) :
    m_environment_spatial_hashmap(area_width, area_height), m_resource_controllers(),
    m_month(-1)
{

}

EnvironmentManager::~EnvironmentManager()
{

}

int EnvironmentManager::getDailyIllumination(QPoint p_center, int p_id, float p_canopy_width, float height)
{
    return m_resource_controllers.illumination.getDailyIllumination(m_environment_spatial_hashmap, p_center, p_id, p_canopy_width, height);
}

int EnvironmentManager::getSoilHumidity(QPoint p_center, float p_roots_size, int p_id)
{
    return m_resource_controllers.soil_humidity.getSoilHumidity(m_environment_spatial_hashmap, p_center, p_roots_size, p_id);
}

int EnvironmentManager::getTemperature()
{
    return m_temperatures.at(m_month-1);
}

void EnvironmentManager::remove(QPoint p_center, float p_canopy_width, float p_roots_size, int p_id)
{
    if(p_canopy_width > 0)
        m_resource_controllers.illumination.remove(m_environment_spatial_hashmap, p_center, p_canopy_width, p_id);
    m_resource_controllers.soil_humidity.remove(m_environment_spatial_hashmap, p_center, p_roots_size, p_id);
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
        m_resource_controllers.illumination.update(m_environment_spatial_hashmap, p_center, p_canopy_width, p_height, p_id);

    // Update soil humidity
    m_resource_controllers.soil_humidity.update(m_environment_spatial_hashmap, p_center, p_roots_size, p_id, p_minimum_soil_humidity_request);
}


void EnvironmentManager::setEnvironmentProperties( float slope, std::vector<int> humidity, std::vector<int> illumination, std::vector<int> temperature )
{
    m_environment_spatial_hashmap.resetAllCells();

    m_humidities = humidity;
    m_illuminations = illumination;
    m_temperatures = temperature;
}

void EnvironmentManager::setMonth(int p_month)
{
    m_month = p_month;
    m_environment_spatial_hashmap.setAvailableResources(m_illuminations.at(p_month-1), m_humidities.at(p_month-1), m_temperatures.at(p_month-1));
}

std::vector<int> EnvironmentManager::getHumidities() const
{
    return m_humidities;
}

std::vector<int> EnvironmentManager::getIlluminations() const
{
    return m_illuminations;
}

std::vector<int> EnvironmentManager::getTemperatures() const
{
    return m_temperatures;
}

