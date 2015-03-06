#ifndef CONSTRAINERS_H
#define CONSTRAINERS_H

#include "plant_properties.h"
#include <memory>
#include "helper.h"
#include "constants.h"

class Constrainer
{
public:
    virtual ~Constrainer() {}

    virtual int getStrength() const = 0;
};

/*******************
 * AGE CONSTRAINER *
 *******************/
class AgeConstrainer : public Constrainer
{
public:
    AgeConstrainer(const AgeingProperties * p_ageing_properties);
    ~AgeConstrainer();

    virtual int getStrength() const ;

    void setAge(int p_age_in_months);

private:
    int m_age;
    AgeingProperties m_properties;
    LinearEquation m_pre_prime_equation;
    LinearEquation m_post_prime_equation;
};

/*********************
 * SHADE CONSTRAINER *
 *********************/
class IlluminationConstrainer : public Constrainer
{
public:
    IlluminationConstrainer(const IlluminationProperties * p_illumination_properties);
    ~IlluminationConstrainer();

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
    SoilHumidityConstrainer(const SoilHumidityProperties * p_humidity_properties);
    ~SoilHumidityConstrainer();

    virtual int getStrength() const;
    bool isInDrought() const;

    void setSoilHumidityPercentage(int p_soil_humidity_percentage);
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
    TemperatureConstrainer(const TemperatureProperties * p_temperature_properties);
    ~TemperatureConstrainer();

    virtual int getStrength() const;

    void setGroundTemperature(int p_ground_temperature);

private:
    LinearEquation m_coldness_equation;
    LinearEquation m_heat_equation;
    TemperatureProperties m_properties;
    int m_ground_temperature;
};

#endif //CONSTRAINERS_H
