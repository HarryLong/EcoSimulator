#ifndef GROWTH_MANAGER_H
#define GROWTH_MANAGER_H

struct GrowthProperties {
    float monthly_growth; // cm per month
    float height_to_radius_ratio;

    GrowthProperties( float p_monthly_growth, float p_height_to_width_ratio) : monthly_growth(p_monthly_growth), height_to_radius_ratio(p_height_to_width_ratio) {}
};

class GrowthManager
{
public:
    GrowthManager(const GrowthProperties * p_growth_properties);

    float getHeight() const { return m_height; }
    float getCanopyRadius() const { return m_height * m_growth_properties->height_to_radius_ratio; }
    void grow(float p_strength); // Must be called monthly!

private:
    const GrowthProperties * m_growth_properties;
    float m_height;
};

#endif // GROWTH_MANAGER_H
