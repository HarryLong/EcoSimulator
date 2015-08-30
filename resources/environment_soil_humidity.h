#ifndef ENVIRONMNENT_SOIL_HUMIDITY_H
#define ENVIRONMNENT_SOIL_HUMIDITY_H

#include <QPoint>
#include <QImage>
#include <memory>
#include "../data_holders/environment_spatial_hashmap.h"

class EnvironmentSoilHumidity
{
public:
    static const int _MIN = 0;
    static const int _MAX = 1000;

    EnvironmentSoilHumidity(EnvironmentSpatialHashMap & map);
    void setSoilHumidityData(int humidity[12]);
    int getSoilHumidity(QPoint p_center, float p_roots_size, int p_id);
    void update(QPoint p_center, float p_roots_size, int p_id, int p_minimum_humidity);
    void remove(QPoint p_center, float p_roots_size, int p_id);

private:
    EnvironmentSpatialHashMap & m_map;
};

#endif //ENVIRONMNENT_SOIL_HUMIDITY_H
