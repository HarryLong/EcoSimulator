#ifndef ILLUMINATION_CONSTRAINER_H
#define ILLUMINATION_CONSTRAINER_H
#include <memory>
#include <algorithm>
#include "helper.h"

struct IlluminationProperties {
    int avg_max_shade;

    IlluminationProperties(int p_avg_max_cover) : avg_max_shade(std::max(p_avg_max_cover,1)) {}
};

class IlluminationConstrainer
{
public:
    IlluminationConstrainer(std::shared_ptr<IlluminationProperties> p_illumination_properties);
    ~IlluminationConstrainer();

    int getStrength(float p_ratio);

private:
    std::shared_ptr<IlluminationProperties> m_illumination_properties;
    LinearEquation * m_equation;
};

#endif // ILLUMINATION_CONSTRAINER_H
