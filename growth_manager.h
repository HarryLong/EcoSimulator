#ifndef GROWTH_MANAGER_H
#define GROWTH_MANAGER_H

#include <memory>
#include "plant_properties.h"
#include "dice_roller.h"

class GrowthManager
{
public:
    GrowthManager(const GrowthProperties * p_growth_properties);

    float getHeight() const { return m_height; }
    float getCanopyRadius() const { return (m_height * m_growth_properties.height_to_width_multiplier)/2; }
    float getRootsRadius() const { return m_root_radius; }
    void grow(int p_strength); // Must be called monthly!

private:
    GrowthProperties m_growth_properties;
    float m_height;
    float m_root_radius;
    DiceRoller m_dice_roller;
};

#endif // GROWTH_MANAGER_H
