#ifndef CONSTRAINERS_H
#define CONSTRAINERS_H

#include "plant_properties.h"
#include <memory>
#include "helper.h"

class Constrainer
{
public:
    virtual ~Constrainer() {}

    virtual float getStrength() = 0;
};

class AgeConstrainer : public Constrainer
{
public:
    AgeConstrainer(std::shared_ptr<AgeingProperties> p_ageing_properties);
    ~AgeConstrainer();

    virtual float getStrength();

    void setAge(int p_age_in_months);

private:
    int m_age;
    int m_prime_age;
    LinearEquation * m_pre_prime_equation;
    LinearEquation * m_post_prime_equation;
};

class IlluminationConstrainer : public Constrainer
{
public:
    IlluminationConstrainer(std::shared_ptr<IlluminationProperties> p_illumination_properties);
    ~IlluminationConstrainer();

    virtual float getStrength();

    void setShadedRatio(float p_shaded_ratio);

private:
    float m_ratio_start_of_negative_impact;
    LinearEquation * m_equation;
    float m_shaded_ratio;
};

#endif //CONSTRAINERS_H
