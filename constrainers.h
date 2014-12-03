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

    virtual int getStrength() = 0;
};

/*******************
 * AGE CONSTRAINER *
 *******************/
class AgeConstrainer : public Constrainer
{
public:
    AgeConstrainer(std::shared_ptr<AgeingProperties> p_ageing_properties);
    ~AgeConstrainer();

    virtual int getStrength();

    void setAge(int p_age_in_months);

private:
    std::shared_ptr<AgeingProperties> m_properties;
    int m_age;
    LinearEquation * m_pre_prime_equation;
    LinearEquation * m_post_prime_equation;
};

/****************************
 * ILLUMINATION CONSTRAINER *
 ****************************/
class IlluminationConstrainer : public Constrainer
{
public:
    IlluminationConstrainer(std::shared_ptr<IlluminationProperties> p_illumination_properties);
    ~IlluminationConstrainer();

    virtual int getStrength();

    void setShadedPercentage(int p_shaded_percentage);

private:
    std::shared_ptr<IlluminationProperties> m_properties;
    LinearEquation * m_equation;
    int m_shaded_percentage;
};

/*****************
 * SOIL HUMIDITY *
 *****************/
class SoilHumidityConstrainer : public Constrainer
{
public:
    SoilHumidityConstrainer(std::shared_ptr<SoilHumidityProperties> p_humidity_properties);
    ~SoilHumidityConstrainer();

    virtual int getStrength();
    bool isInDrought();

    void setSoilHumidityPercentage(int p_soil_humidity_percentage);
    int getMinimumPrimeSoilHumidity();

private:
    LinearEquation * m_drought_equation;
    LinearEquation * m_flood_equation;
    std::shared_ptr<SoilHumidityProperties> m_properties;
    int m_soil_humidity;
};

#endif //CONSTRAINERS_H
