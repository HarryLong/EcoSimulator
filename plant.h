#ifndef PLANT_H
#define PLANT_H

#include <string>
#include <QColor>
#include "helper.h"
#include <unordered_set>
#include "plants_storage.h"

class AgeConstrainer;
class IlluminationConstrainer;
class GrowthManager;

struct Strengths
{
    int age;
    int illumination;

    int getMin() { return min(age, illumination); }

    Strengths() : age(1000), illumination(1000) {}
};

class Plant {
public:
    Plant(std::string p_name, QColor p_color, Coordinate p_center_coord, long p_unique_id, int p_random_id,
          GrowthManager * p_growth_manager, AgeConstrainer * p_age_constrainer,
          IlluminationConstrainer * p_illumination_constrainer);
    ~Plant();

    void newMonth(); // Returns the radius increase

    float getHeight() const;
    float getCanopyRadius() const;

    bool survives() {
        if(m_random_id < m_strengths.getMin())
            return true;
        else
        {
            if(m_strengths.age < m_strengths.illumination)
                std::cout << "DEATH BY AGE!" << std::endl;
            else
                std::cout << "DEATH BY ILLUMINATION" << std::endl;
            return false;
        }
    }

    const std::string m_name;
    const QColor m_color;
    const int m_unique_id;
    const Coordinate m_center_position;

    void addIntersectingPlantId(int p_plant_id);
    void removeIntersectingPlantId(int p_plant_id);
    const std::unordered_set<int> getIntersectingPlantIds() const { return m_intersecting_plant_ids; }

    void addToWorldIndices(int index);
    const std::vector<int> & getWorldIndices() const { return m_world_indices; }

    void calculateStrength(const PlantStorage & p_plant_storage);
private:
    void calculate_new_positions();
    float calculate_intersection_area(const Coordinate & p_circle_center, float p_circle_radius);

    // Managers
    GrowthManager * m_growth_manager;

    // Constrainers
    AgeConstrainer * m_age_constrainer;
    IlluminationConstrainer * m_illumination_constrainer;

    std::vector<int> m_world_indices;
    std::unordered_set<int> m_intersecting_plant_ids;

    Strengths m_strengths;
    int m_random_id;

    int previously_appended_interesecting_plant_id;
};

#endif //PLANT_H
