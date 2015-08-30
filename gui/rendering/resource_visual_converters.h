#ifndef RESOURCE_VISUAL_CONVERTERS_H
#define RESOURCE_VISUAL_CONVERTERS_H

#include <QColor>

class ResourceConverter {
public:
    ResourceConverter(int min, int max);
    virtual ~ResourceConverter();
    virtual int toValue(QRgb p_pixel) const = 0;
    virtual QRgb toRGB(int p_value) const = 0 ;

protected:
    int m_min, m_max, m_range;
};

/*****************
 * SOIL HUMIDITY *
 *****************/
class SoilHumidityConverter : public ResourceConverter
{
public:
    SoilHumidityConverter();
    ~SoilHumidityConverter();

    virtual int toValue(QRgb p_pixel) const;
    virtual QRgb toRGB(int p_value) const;
};

/****************
 * ILLUMINATION *
 ****************/
class IlluminationConverter : public ResourceConverter
{
public:
    IlluminationConverter();
    ~IlluminationConverter();

    virtual int toValue(QRgb p_pixel) const;
    virtual QRgb toRGB(int p_value) const;
};

/***************
 * TEMPERATURE *
 ***************/
class TemperatureConverter : public ResourceConverter
{
public:
    TemperatureConverter();
    ~TemperatureConverter();

    virtual int toValue(QRgb p_pixel) const;
    virtual QRgb toRGB(int p_value) const;
};

#endif // RESOURCE_VISUAL_CONVERTERS_H
