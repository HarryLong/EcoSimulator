#ifndef PLANT_PROPERTIES_H
#define PLANT_PROPERTIES_H
#include <QString>

struct AgeingProperties {
    const int probability_of_death_at_birth;
    const int probability_of_death_at_upper;

    const int prime_age_start;
    const int prime_age_end;
    const int upper_age;

    AgeingProperties(const AgeingProperties & other):
        probability_of_death_at_birth(other.probability_of_death_at_birth),
        probability_of_death_at_upper(other.probability_of_death_at_upper),
        prime_age_start(other.prime_age_start),
        prime_age_end(other.prime_age_end),
        upper_age(other.upper_age) {}


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
    const float max_annual_vertical_growth; // cm per month
    const float max_annual_root_growth; // cm per month
    const float height_to_width_multiplier; // cm per month

    GrowthProperties(const GrowthProperties& other) :
        max_annual_vertical_growth(other.max_annual_vertical_growth),
        max_annual_root_growth(other.max_annual_root_growth),
        height_to_width_multiplier(other.height_to_width_multiplier) {}

    GrowthProperties( float p_max_annual_vertical_growth, float p_max_annual_root_growth,
                      float p_height_to_width_multiplier) :
        max_annual_vertical_growth(p_max_annual_vertical_growth),
        max_annual_root_growth(p_max_annual_root_growth),
        height_to_width_multiplier(p_height_to_width_multiplier) {}
};

struct IlluminationProperties {
    const int shade_tolerance;
    const int sensitivity;

    IlluminationProperties(const IlluminationProperties & other) :
        shade_tolerance(other.shade_tolerance),
        sensitivity(other.sensitivity){}

    IlluminationProperties( int p_shade_tolerance, int p_sensitivity ) :
        shade_tolerance(p_shade_tolerance),
        sensitivity(p_sensitivity){}
};

struct SoilHumidityProperties {
    const int soil_humidity_percentage_prime_start;
    const int soil_humidity_percentage_prime_end;
    const int sensitivity;

    SoilHumidityProperties(const SoilHumidityProperties & other) :
        soil_humidity_percentage_prime_start(other.soil_humidity_percentage_prime_start),
        soil_humidity_percentage_prime_end(other.soil_humidity_percentage_prime_end),
        sensitivity(other.sensitivity){}

    SoilHumidityProperties( int p_soil_humidity_percentage_prime_start,
                            int p_soil_humidity_percentage_prime_end,
                            int p_sensitivity):
        soil_humidity_percentage_prime_start(p_soil_humidity_percentage_prime_start),
        soil_humidity_percentage_prime_end(p_soil_humidity_percentage_prime_end),
        sensitivity(p_sensitivity){}
};

struct TemperatureProperties {
    const int temperature_range_start;
    const int temperature_range_end;
    const int sensitivity;

    TemperatureProperties(const TemperatureProperties & other) :
        temperature_range_start(other.temperature_range_start),
        temperature_range_end(other.temperature_range_end),
        sensitivity(other.sensitivity){}

    TemperatureProperties( int p_temperature_range_start,
                            int p_temperature_range_end,
                            int p_sensitivity):
        temperature_range_start(p_temperature_range_start),
        temperature_range_end(p_temperature_range_end),
        sensitivity(p_sensitivity){}
};

struct SpecieProperties{
    int specie_id;
    const QString specie_name;
    const AgeingProperties* ageing_properties;
    const GrowthProperties* growth_properties;
    const IlluminationProperties* illumination_properties;
    const SoilHumidityProperties* soil_humidiry_properties;
//    const TemperatureProperties* temperature_properties;

    SpecieProperties(const SpecieProperties & other) :
        specie_id(other.specie_id),
        specie_name(other.specie_name),
        ageing_properties(other.ageing_properties),
        growth_properties(other.growth_properties),
        illumination_properties(other.illumination_properties),
        soil_humidiry_properties(other.soil_humidiry_properties)
//        temperature_properties(other.temperature_properties)
    {}


    SpecieProperties(QString name, int id,
              const AgeingProperties* ageing_properties,
              const GrowthProperties* growth_properties,
              const IlluminationProperties* illumination_properties,
              const SoilHumidityProperties* soil_humidiry_properties) :
//              const TemperatureProperties* temperature_properties) :
        specie_id(id), specie_name(name),
        ageing_properties(ageing_properties),
        growth_properties(growth_properties),
        illumination_properties(illumination_properties),
        soil_humidiry_properties(soil_humidiry_properties)
//        temperature_properties(temperature_properties)
    {}

    SpecieProperties(QString name,
                    const AgeingProperties* ageing_properties,
                    const GrowthProperties* growth_properties,
                    const IlluminationProperties* illumination_properties,
                    const SoilHumidityProperties* soil_humidity_properties) :
//                    const TemperatureProperties* temperature_properties) :
        specie_id(-1), specie_name(name),
        ageing_properties(ageing_properties),
        growth_properties(growth_properties),
        illumination_properties(illumination_properties),
        soil_humidiry_properties(soil_humidity_properties)
//        temperature_properties(temperature_properties)
        {}

    ~SpecieProperties()
    {
        delete ageing_properties;
        delete growth_properties;
        delete illumination_properties;
        delete soil_humidiry_properties;
//        delete temperature_properties;
    }
};


#endif
