#ifndef CONSTRAINERS_H
#define CONSTRAINERS_H

#include "plantDB/plant_properties.h"
#include "../../math/linear_equation.h"

#include <memory>

class Constrainer
{
public:
    static const int _MIN_STRENGTH;
    static const int _MAX_STRENGTH;

    virtual ~Constrainer() {}
    virtual int getStrength() const = 0;
};

/*******************
 * AGE CONSTRAINER *
 *******************/
class AgeConstrainer : public Constrainer
{
public:
    AgeConstrainer(const AgeingProperties & p_ageing_properties);
    ~AgeConstrainer();
//    AgeConstrainer & operator=(const AgeConstrainer& other);

    virtual int getStrength() const ;

    void setAge(int p_age);

private:
    int m_age;
    AgeingProperties m_properties;
    LinearEquation m_ageing_equation;
};

/*********************
 * SHADE CONSTRAINER *
 *********************/
class IlluminationConstrainer : public Constrainer
{
public:
    IlluminationConstrainer(const IlluminationProperties & p_illumination_properties);
    ~IlluminationConstrainer();
//    IlluminationConstrainer & operator=(const IlluminationConstrainer& other);

    virtual int getStrength() const;

    void setDailyIllumination(int p_daily_illumination);

    bool isUnderExposed();

private:
    IlluminationProperties m_properties;
    LinearEquation m_underexposure_equation;
    LinearEquation m_overexposure_equation;
    int m_daily_illumination; // in hours
};

/*****************
 * SOIL HUMIDITY *
 *****************/
class SoilHumidityConstrainer : public Constrainer
{
public:
    SoilHumidityConstrainer(const SoilHumidityProperties & p_humidity_properties);
    ~SoilHumidityConstrainer();
//    SoilHumidityConstrainer & operator=(const SoilHumidityConstrainer& other);

    virtual int getStrength() const;
    bool isInDrought() const;

    void setSoilHumidity(int p_soil_humidity);
    int getMinimumPrimeSoilHumidity() const;

private:
    LinearEquation m_drought_equation;
    LinearEquation m_flood_equation;
    SoilHumidityProperties m_properties;
    int m_soil_humidity;
};

/***************
 * TEMPERATURE *
 ***************/
class TemperatureConstrainer : public Constrainer
{
public:
    TemperatureConstrainer(const TemperatureProperties & p_temperature_properties);
    ~TemperatureConstrainer();
//    TemperatureConstrainer & operator=(const TemperatureConstrainer& other);


    virtual int getStrength() const;
    bool isTooCold() const;

    void setTemperature(int p_temp);

private:
    LinearEquation m_chill_equation;
    LinearEquation m_warmth_equation;
    TemperatureProperties m_properties;
    int m_temperature;
};

/*********
 * SLOPE *
 *********/
class SlopeConstrainer : public Constrainer
{
public:
    SlopeConstrainer(const SlopeProperties & p_temperature_properties);
    ~SlopeConstrainer();
//    TemperatureConstrainer & operator=(const TemperatureConstrainer& other);


    virtual int getStrength() const;

    void setSlope(int p_slope);

private:
    LinearEquation m_slope_equation;
    SlopeProperties m_properties;
    int m_slope;
};

/***********
 * WRAPPER *
 ***********/
class ConstrainersWrapper{
public:
    ConstrainersWrapper(AgeConstrainer age_constrainer,
                        IlluminationConstrainer illumination_constrainer,
                        SoilHumidityConstrainer soil_humidity_constrainer,
                        TemperatureConstrainer temp_constrainer,
                        SlopeConstrainer slope_constrainer);
    ~ConstrainersWrapper();
//    ConstrainersWrapper & operator=(const ConstrainersWrapper& other);

    AgeConstrainer age_constrainer;
    IlluminationConstrainer illumination_constrainer;
    SoilHumidityConstrainer soil_humidity_constrainer;
    TemperatureConstrainer temp_constrainer;
    SlopeConstrainer slope_constrainer;
};

#endif //CONSTRAINERS_H
