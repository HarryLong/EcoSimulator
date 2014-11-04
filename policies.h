#ifndef POLICIES_H
#define POLICIES_H

#include "helper.h"

/*******************
 * AGEING POLICIES *
 *******************/
struct AgeingProperties {
    float probability_of_death_at_birth;
    float probabiity_of_death_at_prime;
    float probability_of_death_at_max;

    int avg_prime_age;
    int avg_max_age;

    AgeingProperties(float p_probability_of_death_at_birth, float p_probabiity_of_death_at_prime, float p_probability_of_death_at_max,
                     int p_avg_prime_age, int p_avg_max_age) : probability_of_death_at_birth(p_probability_of_death_at_birth),
        probabiity_of_death_at_prime(p_probabiity_of_death_at_prime), probability_of_death_at_max(p_probability_of_death_at_max),
        avg_prime_age(p_avg_prime_age), avg_max_age(p_avg_max_age) {}
};

class AgeingPolicy
{
protected:
    AgeingPolicy(AgeingProperties p_ageing_properties);
    ~AgeingPolicy();

    float get_probability_of_dying_of_age();

    int m_age;
    AgeingProperties m_ageing_properties;
    LinearEquation * m_pre_prime_equation;
    LinearEquation * m_post_prime_equation;
};

// TODO: Potentially create a policy factory
class OakTreeAgeingPolicy : public AgeingPolicy
{
protected:
    OakTreeAgeingPolicy() : AgeingPolicy(AgeingProperties(
                                                 .1f,   // probability of death at birth
                                                 .001f, // probability of death at prime
                                                 .8f,   // probability of death at max
                                                 240,   // average prime age
                                                 1200   // average max age
                                                 )) {}
};

/*******************
 * GROWTH POLICIES *
 *******************/
//TODO: Change this, growth shouldn't be linear
struct GrowthProperties {
    float monthly_factor; // cm per month
    float height_to_radius_ratio;

    GrowthProperties( float p_monthly_growth, float p_height_to_width_ratio) : monthly_factor(p_monthly_growth), height_to_radius_ratio(p_height_to_width_ratio) {}
};

class GrowthPolicy
{
public:
    float getHeight() const { return m_height; }
    float getCanopyRadius() const { return m_height * m_growth_properties.height_to_radius_ratio; }

protected:
    GrowthPolicy(GrowthProperties p_growth_properties);
    void grow(float p_strength); // Must be called monthly!
    GrowthProperties m_growth_properties;

private:
    float m_height;
};

class OakTreeGrowthPolicy : public GrowthPolicy
{
protected:
    OakTreeGrowthPolicy() : GrowthPolicy(GrowthProperties(
                                             .8f, // cm per month
                                             .5f  // Width is half the height
                                             )) {}
};

#endif // POLICIES_H
