#ifndef SIMULATION_CONFIGURATION_H
#define SIMULATION_CONFIGURATION_H

#include <map>
#include <QImage>
#include <array>

struct SimulationConfiguration {
    std::map<int, int> m_plants_to_generate;
    float m_slope;
    std::vector<int> m_humidity;
    std::vector<int> m_illumination;
    std::vector<int> m_temperature;

    int m_duration;
    bool m_seeding_enabled;

    SimulationConfiguration() {}

    SimulationConfiguration(std::map<int, int> plants_to_generate,
                            float slope,
                            std::vector<int> humidity,
                            std::vector<int> illumination,
                            std::vector<int> temperature,
                            int duration = -1,
                            bool enable_seeding = true) :
        m_plants_to_generate(plants_to_generate),
        m_slope(slope),
        m_humidity(humidity),
        m_illumination(illumination),
        m_temperature(temperature),
        m_duration(duration),
        m_seeding_enabled(enable_seeding)
    {}
};

#endif //SIMULATION_CONFIGURATION_H
