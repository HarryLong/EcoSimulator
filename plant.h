#ifndef PLANT_H
#define PLANT_H

#include <string>
#include <QColor>
#include "helper.h"
#include <unordered_set>
#include "plants_storage.h"

class GrowthManager;

class AgeingProperties;
class IlluminationProperties;
class GrowthProperties;

class AgeConstrainer;
class IlluminationConstrainer;

enum ConstrainerType{
    Age,
    Illumination
};

typedef std::map<ConstrainerType, float> Strengths;

struct Constrainers{
    AgeConstrainer * age_constrainer;
    IlluminationConstrainer * illumination_constrainer;
};

class Plant {
public:
    Plant(std::string p_name, QColor p_color, Coordinate p_center_coord, long p_unique_id, int p_random_id,
          std::shared_ptr<GrowthProperties> p_growth_properties, std::shared_ptr<AgeingProperties> p_ageing_properties,
          std::shared_ptr<IlluminationProperties> p_illumination_properties);
    ~Plant();

    void newMonth();

    float getHeight() const;
    float getCanopyRadius() const;

    PlantStatus getStatus();
    void calculateStrength(float p_shaded_ratio);

    const std::string m_name;
    const QColor m_color;
    const int m_unique_id;
    const Coordinate m_center_position;

private:
    // Managers
    GrowthManager * m_growth_manager;

    // Constrainers
    Constrainers m_constrainers;

    Strengths m_strengths;
    ConstrainerType m_strength_bottleneck;
    float m_min_strength;

    int m_random_id; // Random number between 0 and 1000 used for statistical purposes
    int m_age;
};

#endif //PLANT_H
