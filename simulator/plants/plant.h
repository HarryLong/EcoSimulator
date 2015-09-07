#ifndef PLANT_H
#define PLANT_H

#include <string>
#include <QColor>
#include <QPoint>
#include <unordered_set>
#include "plants_storage.h"
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

struct Constrainers{
    AgeConstrainer age_constrainer;
    IlluminationConstrainer illumination_constrainer;
    SoilHumidityConstrainer soil_humidity_constrainer;
    TemperatureConstrainer temp_constrainer;

    Constrainers( AgeConstrainer age_constrainer,
                  IlluminationConstrainer illumination_constrainer,
                  SoilHumidityConstrainer soil_humidity_constrainer,
                  TemperatureConstrainer temp_constrainer):
        age_constrainer(age_constrainer),
        illumination_constrainer(illumination_constrainer),
        soil_humidity_constrainer(soil_humidity_constrainer),
        temp_constrainer(temp_constrainer){}
};

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

    Plant(const SpecieProperties * m_specie_properties, std::shared_ptr<const QColor> p_color, QPoint p_center_coord, long p_unique_id, int p_random_id);
    ~Plant();

    void newMonth();

    float getHeight() const;
    float getCanopyWidth() const;
    float getRootSize() const;
    int getMinimumSoilHumidityRequirement() const;
    std::vector<QPoint> seed();
    std::vector<QPoint> seed(int seed_count);
    int getVigor() const;
    const QColor * getColor() const;

    PlantStatus getStatus();
    void calculateStrength(int p_daily_illumination, int p_soil_humidity_percentage, int p_temp);

    const long m_unique_id;
    const QPoint m_center_position;
    const QString m_specie_name;
    const int m_specie_id;
    const std::shared_ptr<const QColor> m_color;

private:
    // Managers
    GrowthManager m_growth_manager;

    const SeedingProperties * m_seeding_properties;

    // Constrainers
    Constrainers m_constrainers;

    Strengths m_strengths;
    ConstrainerType m_strength_bottleneck;

    int m_strength;
    int m_random_id; // Random number between 0 and 1000 used for statistical purposes
    int m_age;

    int m_pain_enducer;
};

#endif //PLANT_H