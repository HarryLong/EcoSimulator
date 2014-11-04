#ifndef PLANT_H
#define PLANT_H

#include "eco_data.h"
#include <string>
#include <QRgb>
#include "growth_manager.h"
#include "age_constrainer.h"
#include "illumination_constrainer.h"

class Plant {
public:
    Plant(std::string p_name, QRgb p_color, Coordinate p_center_coord, long p_unique_id, int p_random_id,
          GrowthManager * p_growth_manager, AgeConstrainer * p_age_constrainer,
          IlluminationConstrainer * p_illumination_constrainer);
    ~Plant();

    void newMonth();

    void setShadowedPointsRatio(float ratio) {
       std::cout << "Ratio: " << ratio << "| Random id: " << m_random_id << std::endl;
        m_shadowed_points_ratio = ratio;
    }

    float getHeight() const { return m_growth_manager->getHeight(); }
    const CoordinateHolder & getNewPositions() const { return m_new_positions;}
    const CoordinateHolder & getPositions() const { return m_positions;}
    bool survives() { return m_random_id < m_overall_strength ;}

    const std::string m_name;
    const QRgb m_color;
    const int m_unique_id;
    const Coordinate m_center_position;
private:
    void calculate_new_positions();
    void calculate_overall_strength();

    // All this could be moved into a seperate sizeing class
    CoordinateHolder m_positions;
    CoordinateHolder m_new_positions;
    float m_shadowed_points_ratio;
    int m_current_canopy_radius; // in pixels

    // Managers
    GrowthManager * m_growth_manager;

    // Constrainers
    AgeConstrainer * m_age_constrainer;
    IlluminationConstrainer * m_illumination_constrainer;

    float m_overall_strength;
    int m_random_id;
};

#endif //PLANT_H
