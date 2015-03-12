#ifndef ENVIRONMENT_MANAGER_H
#define ENVIRONMENT_MANAGER_H

#include "enviromnent_illumination.h"
#include "environment_soil_humidity.h"
#include "environment_temp.h"
#include "environment_spatial_hashmap.h"

struct ResourceControllers{
    EnvironmentIllumination illumination;
    EnvironmentSoilHumidity soil_humidity;
    EnvironmentTemperature temperature;

    ResourceControllers(EnvironmentSpatialHashMap & map) : illumination(map), soil_humidity(map), temperature(map) {}
};

class EnvironmentManager{
public:
    EnvironmentManager();
    ~EnvironmentManager();

    int getDailyIllumination(QPoint p_center, float p_canopy_width, float p_height);
    int getSoilHumidityPercentage(QPoint p_center, float p_roots_size, int p_id);
    int getTemperature(QPoint p_center);

    void remove(QPoint p_center, float p_canopy_width, float p_roots_size, int p_id);
    void reset();

    void updateEnvironment(QPoint p_center, float p_canopy_width, float p_height, float p_roots_size, int p_id, int p_minimum_soil_humidity_request);

    const EnvironmentSpatialHashMap & getRenderingData();
    void setEnvironmentProperties(PixelData * p_illumination_data, PixelData * p_soil_humidity_data,
                                  PixelData * p_temperature_data);

private:
    EnvironmentSpatialHashMap m_spatial_hashmap;
    ResourceControllers m_resource_controllers;
};

#endif // ENVIRONMENT_MANAGER_H
