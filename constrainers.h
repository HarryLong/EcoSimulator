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

    const int m_prime_age_start;
    const int m_prime_age_end;
private:
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

    const int m_ratio_start_of_negative_impact;

private:
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

    const int m_soil_humidity_prime_start_percentage;
    const int m_soil_humidity_prime_end_percentage;
private:
    LinearEquation * m_drought_equation;
    LinearEquation * m_flood_equation;

    int m_soil_humidity;
};

#endif //CONSTRAINERS_H
