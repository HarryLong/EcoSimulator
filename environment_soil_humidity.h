#ifndef ENVIRONMNENT_SOIL_HUMIDITY_H
#define ENVIRONMNENT_SOIL_HUMIDITY_H

#include <QPoint>
#include <QImage>
#include <memory>
#include "environment_spatial_hashmap.h"

class EnvironmentSoilHumidity
{
public:
    EnvironmentSoilHumidity(EnvironmentSpatialHashMap & map);
    void setSoilHumidityData(const QImage & p_image);
    int getHumidityPercentage(QPoint p_center, float p_radius, int p_id);
    void setData(QPoint p_center, float p_radius, int p_id, int p_minimum_humidity_percentage);
    void remove(QPoint p_center, float p_radius, int p_id);

private:
    void refresh_resource_distribution();
    bool m_refresh_required;
    EnvironmentSpatialHashMap & m_map;
};

#endif //ENVIRONMNENT_SOIL_HUMIDITY_H
