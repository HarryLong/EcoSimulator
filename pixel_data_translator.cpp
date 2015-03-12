#include "pixel_data_translator.h"

/****************************
 * SOIL HUMIDITY PIXEL DATA *
 ****************************/
#define MIN_HUMIDITY 0
#define MAX_HUMIDITY 100
#define HUMIDITY_RANGE (MAX_HUMIDITY-MIN_HUMIDITY)
SoilHumidityTranslator::SoilHumidityTranslator()
{
}

SoilHumidityTranslator::~SoilHumidityTranslator()
{
}

QRgb SoilHumidityTranslator::toRGB(int p_value) const
{
    float blue_intensity(((float)(p_value-MIN_HUMIDITY))/HUMIDITY_RANGE);
    return qRgb(0, 0, blue_intensity*255);
}

int SoilHumidityTranslator::toValue(QRgb p_pixel) const
{
    return MIN_HUMIDITY + ((qBlue(p_pixel)/255.0f) * HUMIDITY_RANGE);
}

/***************************
 * ILLUMINATION PIXEL DATA *
 ***************************/
#define MIN_ILLUMINATION 0
#define MAX_ILLUMINATION 24
#define ILLUMINATION_RANGE (MAX_ILLUMINATION-MIN_ILLUMINATION)
IlluminationTranslator::IlluminationTranslator()
{
}

IlluminationTranslator::~IlluminationTranslator()
{
}

QRgb IlluminationTranslator::toRGB(int p_value) const
{
    float red_green_intensity(((float)(p_value-MIN_ILLUMINATION))/ILLUMINATION_RANGE);

    return qRgb(red_green_intensity*255, red_green_intensity*255, 0);
}

int IlluminationTranslator::toValue(QRgb p_pixel) const
{
    return MIN_ILLUMINATION + ((qRed(p_pixel)/255.0f) * ILLUMINATION_RANGE);
}

/***************
 * TEMPERATURE *
 ***************/
#define MIN_TEMP -50
#define MAX_TEMP 50
TemperatureTranslator::TemperatureTranslator()
{
}

TemperatureTranslator::~TemperatureTranslator()
{
}

QRgb TemperatureTranslator::toRGB(int p_value) const
{
    float red_intensity(0);
    float blue_intensity(0);
    if(p_value < 0)
    {
        blue_intensity = (((float)p_value)/MIN_TEMP);
    }
    else if (p_value > 0)
    {
        red_intensity = (((float)p_value)/MAX_TEMP);
    }

    return qRgb(red_intensity*255, 0, blue_intensity*255);
}

int TemperatureTranslator::toValue(QRgb p_pixel) const
{
    int temp(0);
    int red, blue;
    if((red = qRed(p_pixel)) > 0)
    {
        temp = (red/255.0f) * MAX_TEMP;
    }
    else if((blue = qBlue(p_pixel)) > 0)
    {
        temp = (blue/255.0f) * MIN_TEMP;
    }

    return temp;
}
