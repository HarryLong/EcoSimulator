#ifndef ENVIRONMENT_MANAGER_H
#define ENVIRONMENT_MANAGER_H

#include "enviromnent_illumination.h"

struct Factors{
    EnvironmentIllumination illumination;
};

class EnvironmentManager{
public:
    EnvironmentManager();

    float getIlluminationPercentage(Coordinate p_center, float p_radius, float p_height);
    void setSize(Coordinate p_center, float p_radius, float p_height, int p_id);
    void remove(Coordinate p_center, float p_radius, int p_id);
    void reset();

    IlluminationSpatialHashMap getIlluminationRenderingData();

private:
    Factors m_factors;
};

#endif // ENVIRONMENT_MANAGER_H
