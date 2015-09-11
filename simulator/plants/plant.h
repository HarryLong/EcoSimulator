#ifndef PLANT_H
#define PLANT_H

#include <string>
#include <QColor>
#include <QPoint>
#include <unordered_set>
#include "plantDB/plant_properties.h"
#include "growth_manager.h"
#include "constrainers.h"

enum ConstrainerType{
    Age,
    Illumination,
    SoilHumidity,
    Temperature
};

typedef std::map<ConstrainerType, float> Strengths;

class Plant {
public:
    enum PlantStatus{
        Alive,
        DeathByAge,
        DeathByUnderIllumination,
        DeathByOverIllumination,
        DeathByDrought,
        DeathByFlood,
        DeathByCold,
        DeathByHeat
    };

    Plant(SpecieProperties m_specie_properties, QColor p_color, QPoint p_center_coord, long p_unique_id, int p_random_id);
    ~Plant();

//    Plant & operator=(const Plant& other);

    void newMonth();

    float getHeight() const;
    float getCanopyWidth() const;
    float getRootSize() const;
    int getMinimumSoilHumidityRequirement() const;
    std::vector<QPoint> seed();
    std::vector<QPoint> seed(int seed_count);
    int getVigor() const;
    QColor getColor() const;

    PlantStatus getStatus();
    void calculateStrength(int p_daily_illumination, int p_soil_humidity_percentage, int p_temp);

    long m_unique_id;
    QPoint m_center_position;
    QString m_specie_name;
    int m_specie_id;
    const QColor m_color;

private:
    // Managers
    GrowthManager m_growth_manager;

    const SeedingProperties m_seeding_properties;

    // Constrainers
    ConstrainersWrapper m_constrainers;

    Strengths m_strengths;
    ConstrainerType m_strength_bottleneck;

    int m_strength;
    int m_random_id; // Random number between 0 and 1000 used for statistical purposes
    int m_age;

    int m_pain_enducer;
};

#endif //PLANT_H
