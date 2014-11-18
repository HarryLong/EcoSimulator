#include "illumination_constrainer.h"
#include <iostream>

#define MIN_STRENGTH 20

IlluminationConstrainer::IlluminationConstrainer(std::shared_ptr<IlluminationProperties> p_illumination_properties) : m_illumination_properties(p_illumination_properties)
{
    // Build the linear equation
    double a = (1000-MIN_STRENGTH) / m_illumination_properties->avg_max_shade;
    double b = 0;

    m_equation = new LinearEquation(a,b);
}

IlluminationConstrainer::~IlluminationConstrainer()
{
    delete m_equation;
}

/**
 * @brief IlluminationConstrainer::getStrength
 * @param p_ratio --> shadowed / all
 * @return
 */
int IlluminationConstrainer::getStrength(float p_ratio)
{
    int ratio_over_thousand(p_ratio*1000);
    if(ratio_over_thousand > m_illumination_properties->avg_max_shade)
    {
        return MIN_STRENGTH;
    }
    else
    {
        return 1000 - m_equation->calculateY<int>(ratio_over_thousand);
    }
}
