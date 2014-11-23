#ifndef GROWTH_MANAGER_H
#define GROWTH_MANAGER_H

#include <memory>
#include "plant_properties.h"

class GrowthManager
{
public:
    GrowthManager(std::shared_ptr<GrowthProperties> p_growth_properties);

    float getHeight() const { return m_height; }
    float getCanopyRadius() const { return m_height * m_growth_properties->height_to_radius_ratio; }
    void grow(int p_strength); // Must be called monthly!

private:
    std::shared_ptr<GrowthProperties> m_growth_properties;
    float m_height;
};

#endif // GROWTH_MANAGER_H
