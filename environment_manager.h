#ifndef ENVIRONMENT_MANAGER_H
#define ENVIRONMENT_MANAGER_H

#include "enviromnent_illumination.h"
#include "environment_soil_humidity.h"

struct Factors{
    EnvironmentIllumination illumination;
    EnvironmentSoilHumidity soil_humidity;
};

class EnvironmentManager{
public:
    EnvironmentManager();

    int getShadedPercentage(QPoint p_center, float p_canopy_radius, float p_height);
    int getSoilHumidityPercentage(QPoint p_center, float p_roots_radius, int p_id);

    void processResourceRequests();

    void remove(QPoint p_center, float p_canopy_radius, float p_roots_radius, int p_id);
    void reset();

    void updateEnvironment(QPoint p_center, float p_canopy_radius, float p_height, float p_roots_radius, int p_id, int p_minimum_soil_humidity_request);

    IlluminationSpatialHashMap getIlluminationRenderingData();
    SoilHumiditySpatialHashMap getSoilHumidityRenderingData();

    void setIllumination(const QImage* p_illumination_data);
    void setSoilHumidity(const QImage* p_soil_humidity_data);

private:
    Factors m_factors;
};

#endif // ENVIRONMENT_MANAGER_H
