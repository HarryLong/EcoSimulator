#ifndef SIMULATION_CONFIGURATION_H
#define SIMULATION_CONFIGURATION_H

#include <map>
#include <QImage>

struct SimulationOptions{
    SimulationOptions(bool simplified_seeding_enabled, bool per_plant_seeding_enabled) :
        per_plant_seeding_enabled(per_plant_seeding_enabled),
        simplified_seeding_enabled(simplified_seeding_enabled){}

    bool per_plant_seeding_enabled;
    bool simplified_seeding_enabled;
};

struct SimulationConfiguration {
    std::map<int, int> m_plants_to_generate;
    const QImage soil_humidity;
    const QImage illumination;
    const SimulationOptions simulation_options;

    SimulationConfiguration(std::map<int, int> plants_to_generate, QImage soil_humidity, QImage illumination,
                       SimulationOptions simulation_configuration):
        m_plants_to_generate(plants_to_generate),
        soil_humidity(soil_humidity),
        illumination(illumination),
        simulation_options(simulation_configuration)
    {}
};

#endif //SIMULATION_CONFIGURATION_H
