#ifndef PIXEL_TRANSLATOR_H
#define PIXEL_TRANSLATOR_H

#include <QColor>

class PixelDataTranslator {
public:
    virtual ~PixelDataTranslator() {}
    virtual int toValue(QRgb p_pixel) const = 0;
    virtual QRgb toRGB(int p_value) const = 0 ;
};

/*****************
 * SOIL HUMIDITY *
 *****************/
class SoilHumidityTranslator : public PixelDataTranslator
{
public:
    SoilHumidityTranslator();
    ~SoilHumidityTranslator();

    virtual int toValue(QRgb p_pixel) const;
    virtual QRgb toRGB(int p_value) const;
};

/****************
 * ILLUMINATION *
 ****************/
class IlluminationTranslator : public PixelDataTranslator
{
public:
    IlluminationTranslator();
    ~IlluminationTranslator();

    virtual int toValue(QRgb p_pixel) const;
    virtual QRgb toRGB(int p_value) const;
};

/***************
 * TEMPERATURE *
 ***************/
class TemperatureTranslator : public PixelDataTranslator
{
public:
    TemperatureTranslator();
    ~TemperatureTranslator();


    virtual int toValue(QRgb p_pixel) const;
    virtual QRgb toRGB(int p_value) const;
};

#endif // PIXEL_TRANSLATOR_H
