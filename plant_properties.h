#ifndef PLANT_PROPERTIES_H
#define PLANT_PROPERTIES_H

struct AgeingProperties {
    float probability_of_death_at_birth;
    float probabiity_of_death_at_prime;
    float probability_of_death_at_upper;

    int prime_age;
    int upper_age;

    AgeingProperties(int p_probability_of_death_at_birth, int p_probability_of_death_at_upper,
                     int p_prime_age, int p_upper_age) :
        probability_of_death_at_birth(p_probability_of_death_at_birth),
        probability_of_death_at_upper(p_probability_of_death_at_upper),
        prime_age(p_prime_age),
        upper_age(p_upper_age) {}
};

struct GrowthProperties {
    float monthly_growth; // cm per month
    float height_to_radius_ratio;

    GrowthProperties( float p_monthly_growth, float p_height_to_width_ratio) : monthly_growth(p_monthly_growth), height_to_radius_ratio(p_height_to_width_ratio) {}
};

struct IlluminationProperties {
    float ratio_start_of_negative_impact;
    float ratio_max_shade;

    float probability_of_death_at_max_shade;

    IlluminationProperties( float p_ratio_start_of_negative_impact, float p_ratio_max_shade, float p_probability_of_death_at_max_shade) :
        ratio_start_of_negative_impact(p_ratio_start_of_negative_impact),
        ratio_max_shade(p_ratio_max_shade),
        probability_of_death_at_max_shade(p_probability_of_death_at_max_shade) {}
};

#endif
