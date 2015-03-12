#ifndef SIMULATION_CONFIGURATION_H
#define SIMULATION_CONFIGURATION_H

#include <map>
#include <QImage>
#include "input_widgets.h"

struct SimulationOptions{
    SimulationOptions(bool simplified_seeding_v1_enabled, bool simplified_seeding_v2_enabled, bool per_plant_seeding_enabled) :
        simplified_seeding_v1_enabled(simplified_seeding_v1_enabled),
        simplified_seeding_v2_enabled(simplified_seeding_v2_enabled),
            per_plant_seeding_enabled(per_plant_seeding_enabled){}

    bool per_plant_seeding_enabled;
    bool simplified_seeding_v1_enabled;
    bool simplified_seeding_v2_enabled;
};

struct SimulationConfiguration {
    std::map<int, int> m_plants_to_generate;
    PixelData * humidity;
    PixelData * illumination;
    PixelData * temperature;
    const SimulationOptions simulation_options;

    SimulationConfiguration(std::map<int, int> plants_to_generate, PixelData * humidity, PixelData * illumination,
                             PixelData * temperature, SimulationOptions simulation_configuration):
        m_plants_to_generate(plants_to_generate),
        humidity(humidity),
        illumination(illumination),
        temperature(temperature),
        simulation_options(simulation_configuration)
    {}
};

#endif //SIMULATION_CONFIGURATION_H
