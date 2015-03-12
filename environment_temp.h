#ifndef ENVIRONMNENT_TEMPERATURE_H
#define ENVIRONMNENT_TEMPERATURE_H

#include <QPoint>
#include <QImage>
#include <memory>
#include "environment_spatial_hashmap.h"
#include "input_widgets.h"

class EnvironmentTemperature
{
public:
    EnvironmentTemperature(EnvironmentSpatialHashMap & map);
    void setTemperatureData(PixelData * p_data);
    int getTemperature(QPoint p_center);

private:
    EnvironmentSpatialHashMap & m_map;
};

#endif //ENVIRONMNENT_TEMPERATURE_H
