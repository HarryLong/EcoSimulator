#ifndef SIMULATION_CONFIGURATION_H
#define SIMULATION_CONFIGURATION_H

#include <map>
#include <QImage>
#include "input_widgets.h"

struct SimulationOptions{
    SimulationOptions(bool simplified_seeding_enabled, bool per_plant_seeding_enabled) :
        simplified_seeding_enabled(simplified_seeding_enabled),
            per_plant_seeding_enabled(per_plant_seeding_enabled){}

    bool per_plant_seeding_enabled;
    bool simplified_seeding_enabled;
};

struct SimulationConfiguration {
    std::map<int, int> m_plants_to_generate;
    PixelData * min_humidity;
    int humidity_variance;

    PixelData * min_illumination;
    int illumination_variance;

    PixelData * min_temperature;
    int temperature_variance;

    const SimulationOptions simulation_options;

    SimulationConfiguration(std::map<int, int> plants_to_generate,
                            PixelData * min_humidity, int humidity_variance,
                            PixelData * min_illumination, int illumination_variance,
                            PixelData * min_temperature, int temperature_variance,
                            SimulationOptions simulation_configuration):
        m_plants_to_generate(plants_to_generate),
        min_humidity(min_humidity),
        humidity_variance(humidity_variance),
        min_illumination(min_illumination),
        illumination_variance(illumination_variance),
        min_temperature(min_temperature),
        temperature_variance(temperature_variance),
        simulation_options(simulation_configuration)
    {}
};

#endif //SIMULATION_CONFIGURATION_H
