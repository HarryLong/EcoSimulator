#ifndef ENVIRONMENT_MANAGER_H
#define ENVIRONMENT_MANAGER_H

#include "environment_illumination.h"
#include "environment_soil_humidity.h"
#include "environment_temp.h"
#include "../data_holders/environment_spatial_hashmap.h"

struct ResourceControllers{
    EnvironmentIllumination illumination;
    EnvironmentSoilHumidity soil_humidity;
    EnvironmentTemperature temperature;

    ResourceControllers() {}
};

class EnvironmentManager{
public:
    EnvironmentManager(int area_width, int area_height);
    ~EnvironmentManager();

    int getDailyIllumination(QPoint p_center, int p_id, float p_canopy_width, float height);
    int getSoilHumidity(QPoint p_center, float p_roots_size, int p_id);
    int getTemperature();
    float getSlope();

    void setMonth(int p_month);
    void remove(QPoint p_center, float p_canopy_width, float p_roots_size, int p_id);
    void reset();

    void updateEnvironment(QPoint p_center, float p_canopy_width, float p_height, float p_roots_size, int p_id, int p_minimum_soil_humidity_request);

    std::vector<int> getHumidities() const;
    std::vector<int> getIlluminations() const;
    std::vector<int> getTemperatures() const;

    const EnvironmentSpatialHashMap & getRenderingData();
    void setEnvironmentProperties( float slope, std::vector<int> humidity, std::vector<int> illumination, std::vector<int> temperature );
private:
    EnvironmentSpatialHashMap m_environment_spatial_hashmap;
    ResourceControllers m_resource_controllers;

    int m_month;
    int m_slope;
    std::vector<int> m_humidities;
    std::vector<int> m_illuminations;
    std::vector<int> m_temperatures;
};

#endif // ENVIRONMENT_MANAGER_H
