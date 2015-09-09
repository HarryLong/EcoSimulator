#ifndef GROWTH_MANAGER_H
#define GROWTH_MANAGER_H

#include <memory>

#include "plantDB/plant_properties.h"
#include "../../math/dice_roller.h"

class GrowthManager
{
public:
    GrowthManager(const GrowthProperties & p_growth_properties, const AgeingProperties & p_ageing_properties);
    ~GrowthManager();

    float getHeight() const;
    float getCanopyWidth() const;
    float getRootSize() const;
    void grow(int p_strength); // Must be called monthly!

private:
    float m_height;
    float m_root_size;
    float m_canopy_width;
    DiceRoller m_dice_roller;

    float m_max_monthly_height_growth;
    float m_max_monthly_root_growth;
    float m_max_monthly_canopy_growth;
};

#endif // GROWTH_MANAGER_H
