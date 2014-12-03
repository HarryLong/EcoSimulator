#ifndef PLANT_PROPERTIES_H
#define PLANT_PROPERTIES_H

struct AgeingProperties {
    const int probability_of_death_at_birth;
    const int probability_of_death_at_upper;

    const int prime_age_start;
    const int prime_age_end;
    const int upper_age;

    AgeingProperties(int p_probability_of_death_at_birth,
                     int p_probability_of_death_at_upper,
                     int p_prime_age_start, int p_prime_age_end,
                     int p_upper_age) :
        probability_of_death_at_birth(p_probability_of_death_at_birth),
        probability_of_death_at_upper(p_probability_of_death_at_upper),
        prime_age_start(p_prime_age_start),
        prime_age_end(p_prime_age_end),
        upper_age(p_upper_age) {}
};

struct GrowthProperties {
    const float max_monthly_vertical_growth; // cm per month
    const float max_monthly_root_growth; // cm per month
    const float max_monthly_canopy_growth; // cm per month

    GrowthProperties( float p_max_monthly_height_growth, float p_max_monthly_root_growth,
                      float p_height_to_width_ratio) :
        max_monthly_vertical_growth(p_max_monthly_height_growth),
        max_monthly_root_growth(p_max_monthly_root_growth),
        max_monthly_canopy_growth(p_height_to_width_ratio) {}
};

struct IlluminationProperties {
    const int shadowed_percentage_start_of_negative_impact;
    const int probability_of_death_in_complete_shade;

    IlluminationProperties( int p_shadowed_percentage_start_of_negative_impact,
                            int p_probability_of_death_in_complete_shade) :
        shadowed_percentage_start_of_negative_impact(p_shadowed_percentage_start_of_negative_impact),
        probability_of_death_in_complete_shade(p_probability_of_death_in_complete_shade) {}
};

struct SoilHumidityProperties {
    const int soil_humidity_percentage_prime_start;
    const int soil_humidity_percentage_prime_end;

    SoilHumidityProperties( int p_soil_humidity_percentage_prime_start,
                        int p_soil_humidity_percentage_prime_end):
        soil_humidity_percentage_prime_start(p_soil_humidity_percentage_prime_start),
        soil_humidity_percentage_prime_end(p_soil_humidity_percentage_prime_end) {}
};

#endif
