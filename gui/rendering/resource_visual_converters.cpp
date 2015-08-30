#include "resource_visual_converters.h"
#include "../../resources/environment_illumination.h"
#include "../../resources/environment_temp.h"
#include "../../resources/environment_soil_humidity.h"

#include <QDebug>

/******************
 * BASE CONVERTER *
 ******************/
ResourceConverter::ResourceConverter(int min, int max) : m_min(min), m_max(max), m_range(max-min)
{

}

ResourceConverter::~ResourceConverter()
{

}

/*****************
 * SOIL HUMIDITY *
 *****************/
SoilHumidityConverter::SoilHumidityConverter() : ResourceConverter(EnvironmentSoilHumidity::_MIN, EnvironmentSoilHumidity::_MAX)
{
    m_range = 300;
}

SoilHumidityConverter::~SoilHumidityConverter()
{
}

QRgb SoilHumidityConverter::toRGB(int p_value) const
{
    float blue_intensity(((float)(p_value-m_min))/m_range);
    return qRgb(0, 0, blue_intensity*255+1);
}

int SoilHumidityConverter::toValue(QRgb p_pixel) const
{
    return m_min + ((qBlue(p_pixel)/255.0f) * m_range);
}

/****************
 * ILLUMINATION *
 ****************/
IlluminationConverter::IlluminationConverter() : ResourceConverter(EnvironmentIllumination::_MIN, EnvironmentIllumination::_MAX)
{
}

IlluminationConverter::~IlluminationConverter()
{
}

QRgb IlluminationConverter::toRGB(int p_value) const
{
    float red_green_intensity(((float)(p_value-m_min))/m_range);

    return qRgb(red_green_intensity*255+1, red_green_intensity*255+1, 0);
}

int IlluminationConverter::toValue(QRgb p_pixel) const
{
    return m_min + ((qRed(p_pixel)/255.0f) * m_range);
}

/***************
 * TEMPERATURE *
 ***************/
TemperatureConverter::TemperatureConverter() : ResourceConverter(EnvironmentTemperature::_MIN, EnvironmentTemperature::_MAX)
{
}

TemperatureConverter::~TemperatureConverter()
{
}

QRgb TemperatureConverter::toRGB(int p_value) const
{
    float red_intensity(0);
    float blue_intensity(0);
    if(p_value < 0)
    {
        blue_intensity = ((((float)p_value)/m_min) * 255) + 1;
    }
    else if (p_value > 0)
    {
        red_intensity = ((((float)p_value)/m_min) * 255) + 1;
    }

    return qRgb(red_intensity, 0, blue_intensity);
}

int TemperatureConverter::toValue(QRgb p_pixel) const
{
    int temp(0);
    int red, blue;
    if((red = qRed(p_pixel)) > 0)
    {
        temp = (red/255.0f) * m_min;
    }
    else if((blue = qBlue(p_pixel)) > 0)
    {
        temp = (blue/255.0f) * m_max;
    }

    return temp;
}
