#ifndef PLANT_H
#define PLANT_H

#include <string>
#include <QColor>
#include <QPoint>
#include "helper.h"
#include <unordered_set>
#include "plants_storage.h"

class GrowthManager;

class AgeingProperties;
class IlluminationProperties;
class GrowthProperties;
class SoilHumidityProperties;

class AgeConstrainer;
class IlluminationConstrainer;
class SoilHumidityConstrainer;

enum ConstrainerType{
    Age,
    Illumination,
    SoilHumidity
};

typedef std::map<ConstrainerType, float> Strengths;

struct Constrainers{
    AgeConstrainer * age_constrainer;
    IlluminationConstrainer * illumination_constrainer;
    SoilHumidityConstrainer * soil_humidity_constrainer;
};

class Plant {
public:
    Plant(std::string p_name, QColor p_color, QPoint p_center_coord, long p_unique_id, int p_random_id,
          std::shared_ptr<GrowthProperties> p_growth_properties, std::shared_ptr<AgeingProperties> p_ageing_properties,
          std::shared_ptr<IlluminationProperties> p_illumination_properties, std::shared_ptr<SoilHumidityProperties> p_soil_himidity_properties);
    ~Plant();

    void newMonth();

    float getHeight() const;
    float getCanopyRadius() const;
    float getRootsRadius() const;
    int getMinimumSoilHumidityRequirement() const;
    int getVigor() const;

    PlantStatus getStatus();
    void calculateStrength(int p_shaded_percentage, int p_soil_humidity_percentage);

    const std::string m_name;
    const QColor m_color;
    const int m_unique_id;
    const QPoint m_center_position;

private:
    // Managers
    GrowthManager * m_growth_manager;

    // Constrainers
    Constrainers m_constrainers;

    Strengths m_strengths;
    ConstrainerType m_strength_bottleneck;
    int m_min_strength;

    int m_random_id; // Random number between 0 and 1000 used for statistical purposes
    int m_age;
};

#endif //PLANT_H
