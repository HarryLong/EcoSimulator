#ifndef ENVIRONMNENT_TEMPERATURE_H
#define ENVIRONMNENT_TEMPERATURE_H

#include <QPoint>
#include <QImage>
#include <memory>
#include "../data_holders/environment_spatial_hashmap.h"

class PixelData;
class EnvironmentTemperature
{
public:
    static const int _MIN = -50;
    static const int _MAX = 50;

    EnvironmentTemperature();
};

#endif //ENVIRONMNENT_TEMPERATURE_H
