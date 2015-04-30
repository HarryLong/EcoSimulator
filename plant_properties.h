#ifndef PLANT_PROPERTIES_H
#define PLANT_PROPERTIES_H
#include <QString>

typedef std::pair<int,int> Range;

struct AgeingProperties {
    const int start_of_decline;
    const int max_age;

    AgeingProperties(const AgeingProperties & other):
        start_of_decline(other.start_of_decline),
        max_age(other.max_age) {}


    AgeingProperties(int p_start_of_decline,
                     int p_max_age) :
        start_of_decline(p_start_of_decline),
        max_age(p_max_age) {}
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
    const Range prime_illumination;
    const int min_illumination;
    const int max_illumination;

    IlluminationProperties(const IlluminationProperties & other) :
        prime_illumination(other.prime_illumination),
        min_illumination(other.min_illumination),
        max_illumination(other.max_illumination) {}

    IlluminationProperties( Range p_prime_illumination,
                            int p_min_illumination, int p_max_illumination) :
        prime_illumination(p_prime_illumination),
        min_illumination(p_min_illumination),
        max_illumination(p_max_illumination) {}
};

struct SoilHumidityProperties {
    const Range prime_soil_humidity;
    const int min_soil_humidity;
    const int max_soil_humidity;

    SoilHumidityProperties(const SoilHumidityProperties & other) :
        prime_soil_humidity(other.prime_soil_humidity),
        min_soil_humidity(other.min_soil_humidity),
        max_soil_humidity(other.max_soil_humidity) {}

    SoilHumidityProperties( Range p_prime_soil_humidity,
                            int p_min_soil_humidity,
                            int p_max_soil_humidity):
        prime_soil_humidity(p_prime_soil_humidity),
        min_soil_humidity(p_min_soil_humidity),
        max_soil_humidity(p_max_soil_humidity) {}
};

struct TemperatureProperties {
    const Range prime_temp;
    const int min_temp;
    const int max_temp;

    TemperatureProperties(const TemperatureProperties & other) :
        prime_temp(other.prime_temp),
        min_temp(other.min_temp),
        max_temp(other.max_temp){}

    TemperatureProperties( Range p_prime_temp,
                            int p_min_temp,
                            int p_max_temp):
        prime_temp(p_prime_temp),
        min_temp(p_min_temp),
        max_temp(p_max_temp){}
};

struct SeedingProperties {
    const int max_seed_distance;
    const int seed_count;

    SeedingProperties(const SeedingProperties & other) :
        max_seed_distance(other.max_seed_distance),
        seed_count(other.seed_count){}

    SeedingProperties( int p_max_seed_distance,
                       int p_max_seeds):
        max_seed_distance(p_max_seed_distance),
        seed_count(p_max_seeds){}
};

struct SpecieProperties{
    int specie_id;
    const QString specie_name;
    const AgeingProperties* ageing_properties;
    const GrowthProperties* growth_properties;
    const IlluminationProperties* illumination_properties;
    const SoilHumidityProperties* soil_humidity_properties;
    const TemperatureProperties* temperature_properties;
    const SeedingProperties* seeding_properties;

    SpecieProperties(const SpecieProperties & other) :
        specie_id(other.specie_id),
        specie_name(other.specie_name),
        ageing_properties(other.ageing_properties),
        growth_properties(other.growth_properties),
        illumination_properties(other.illumination_properties),
        soil_humidity_properties(other.soil_humidity_properties),
        temperature_properties(other.temperature_properties),
        seeding_properties(other.seeding_properties)
    {}


    SpecieProperties(QString name, int id,
              const AgeingProperties* ageing_properties,
              const GrowthProperties* growth_properties,
              const IlluminationProperties* illumination_properties,
              const SoilHumidityProperties* soil_humidity_properties,
              const TemperatureProperties* temperature_properties,
              const SeedingProperties* seeding_properties) :
        specie_id(id), specie_name(name),
        ageing_properties(ageing_properties),
        growth_properties(growth_properties),
        illumination_properties(illumination_properties),
        soil_humidity_properties(soil_humidity_properties),
        temperature_properties(temperature_properties),
        seeding_properties(seeding_properties)
    {}

    SpecieProperties(QString name,
                    const AgeingProperties* ageing_properties,
                    const GrowthProperties* growth_properties,
                    const IlluminationProperties* illumination_properties,
                    const SoilHumidityProperties* soil_humidity_properties,
                    const TemperatureProperties* temperature_properties,
                    const SeedingProperties* seeding_properties) :
        specie_id(-1), specie_name(name),
        ageing_properties(ageing_properties),
        growth_properties(growth_properties),
        illumination_properties(illumination_properties),
        soil_humidity_properties(soil_humidity_properties),
        temperature_properties(temperature_properties),
        seeding_properties(seeding_properties)
        {}

    ~SpecieProperties()
    {
        delete ageing_properties;
        delete growth_properties;
        delete illumination_properties;
        delete soil_humidity_properties;
        delete temperature_properties;
        delete seeding_properties;
    }
};


#endif
