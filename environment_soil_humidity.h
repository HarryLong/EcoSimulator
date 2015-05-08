#ifndef ENVIRONMNENT_SOIL_HUMIDITY_H
#define ENVIRONMNENT_SOIL_HUMIDITY_H

#include <QPoint>
#include <QImage>
#include <memory>
#include "input_widgets.h"
#include "environment_spatial_hashmap.h"

class EnvironmentSoilHumidity
{
public:
    EnvironmentSoilHumidity(EnvironmentSpatialHashMap & map);
    void setSoilHumidityData(PixelData * p_data, int variance);
    int getHumidityPercentage(QPoint p_center, float p_roots_size, int p_id);
    void setData(QPoint p_center, float p_roots_size, int p_id, int p_minimum_humidity_percentage);
    void remove(QPoint p_center, float p_roots_size, int p_id);
    void refresh_resource_distribution(int month);
    std::pair<int,int> getRange();

private:
    std::pair<int,int> m_range;
    EnvironmentSpatialHashMap & m_map;
};

#endif //ENVIRONMNENT_SOIL_HUMIDITY_H
