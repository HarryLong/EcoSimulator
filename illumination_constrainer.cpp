#include "illumination_constrainer.h"

IlluminationConstrainer::IlluminationConstrainer(const IlluminationProperties * p_illumination_properties) : m_illumination_properties(p_illumination_properties) {}

/**
 * @brief IlluminationConstrainer::getStrength
 * @param p_ratio --> shadowed / all
 * @return
 */
int IlluminationConstrainer::getStrength(float p_ratio)
{
    return 1000 - (m_illumination_properties->illumination_requirements * p_ratio);
}
