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
    const float max_height; // cm per month
    const float max_root_size; // cm per month

    const float max_canopy_width; // cm per month

    GrowthProperties(const GrowthProperties& other) :
        max_height(other.max_height),
        max_root_size(other.max_root_size),
        max_canopy_width(other.max_canopy_width) {}

    GrowthProperties( float p_max_height, float p_max_root_size,
                      float p_max_canopy_width) :
        max_height(p_max_height),
        max_root_size(p_max_root_size),
        max_canopy_width(p_max_canopy_width) {}
};

struct IlluminationProperties {
    const int daily_illumination_hours_prime_start;
    const int daily_illumination_hours_prime_end;
    const int underexposure_sensitivity;
    const int overexposure_sensitivity;

    IlluminationProperties(const IlluminationProperties & other) :
        daily_illumination_hours_prime_start(other.daily_illumination_hours_prime_start),
        daily_illumination_hours_prime_end(other.daily_illumination_hours_prime_end),
        underexposure_sensitivity(other.underexposure_sensitivity),
        overexposure_sensitivity(other.overexposure_sensitivity){}

    IlluminationProperties( int p_daily_illumination_hours_prime_start, int p_daily_illumination_hours_prime_end,
                            int p_underexposure_sensitivity, int p_overexposure_sensitivity ) :
        daily_illumination_hours_prime_start(p_daily_illumination_hours_prime_start),
        daily_illumination_hours_prime_end(p_daily_illumination_hours_prime_end),
        underexposure_sensitivity(p_underexposure_sensitivity),
        overexposure_sensitivity(p_overexposure_sensitivity){}
};

struct SoilHumidityProperties {
    const int soil_humidity_percentage_prime_start;
    const int soil_humidity_percentage_prime_end;
    const int drought_sensitivity;
    const int flooding_sensitivity;

    SoilHumidityProperties(const SoilHumidityProperties & other) :
        soil_humidity_percentage_prime_start(other.soil_humidity_percentage_prime_start),
        soil_humidity_percentage_prime_end(other.soil_humidity_percentage_prime_end),
        drought_sensitivity(other.drought_sensitivity),
        flooding_sensitivity(other.flooding_sensitivity){}

    SoilHumidityProperties( int p_soil_humidity_percentage_prime_start,
                            int p_soil_humidity_percentage_prime_end,
                            int p_drought_sensitivity,
                            int p_flooding_sensitivity):
        soil_humidity_percentage_prime_start(p_soil_humidity_percentage_prime_start),
        soil_humidity_percentage_prime_end(p_soil_humidity_percentage_prime_end),
        drought_sensitivity(p_drought_sensitivity),
        flooding_sensitivity(p_flooding_sensitivity){}
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

struct SeedingProperties {
    const int max_seed_distance;
    const int seeding_interval;
    const int max_seeds;

    SeedingProperties(const SeedingProperties & other) :
        max_seed_distance(other.max_seed_distance),
        seeding_interval(other.seeding_interval),
        max_seeds(other.max_seeds){}

    SeedingProperties( int p_max_seed_distance,
                       int p_seeding_interval,
                       int p_max_seeds):
        max_seed_distance(p_max_seed_distance),
        seeding_interval(p_seeding_interval),
        max_seeds(p_max_seeds){}
};

struct SpecieProperties{
    int specie_id;
    const QString specie_name;
    const AgeingProperties* ageing_properties;
    const GrowthProperties* growth_properties;
    const IlluminationProperties* illumination_properties;
    const SoilHumidityProperties* soil_humidity_properties;
//    const TemperatureProperties* temperature_properties;
    const SeedingProperties* seeding_properties;

    SpecieProperties(const SpecieProperties & other) :
        specie_id(other.specie_id),
        specie_name(other.specie_name),
        ageing_properties(other.ageing_properties),
        growth_properties(other.growth_properties),
        illumination_properties(other.illumination_properties),
        soil_humidity_properties(other.soil_humidity_properties),
        seeding_properties(other.seeding_properties)
    {}


    SpecieProperties(QString name, int id,
              const AgeingProperties* ageing_properties,
              const GrowthProperties* growth_properties,
              const IlluminationProperties* illumination_properties,
              const SoilHumidityProperties* soil_humidity_properties,
              const SeedingProperties* seeding_properties) :
        specie_id(id), specie_name(name),
        ageing_properties(ageing_properties),
        growth_properties(growth_properties),
        illumination_properties(illumination_properties),
        soil_humidity_properties(soil_humidity_properties),
        seeding_properties(seeding_properties)
    {}

    SpecieProperties(QString name,
                    const AgeingProperties* ageing_properties,
                    const GrowthProperties* growth_properties,
                    const IlluminationProperties* illumination_properties,
                    const SoilHumidityProperties* soil_humidity_properties,
                    const SeedingProperties* seeding_properties) :
        specie_id(-1), specie_name(name),
        ageing_properties(ageing_properties),
        growth_properties(growth_properties),
        illumination_properties(illumination_properties),
        soil_humidity_properties(soil_humidity_properties),
        seeding_properties(seeding_properties)
        {}

    ~SpecieProperties()
    {
        delete ageing_properties;
        delete growth_properties;
        delete illumination_properties;
        delete soil_humidity_properties;
        delete seeding_properties;
    }
};


#endif
