#ifndef PLANT_H
#define PLANT_H

#include <string>
#include <QColor>
#include <QPoint>
#include "helper.h"
#include <unordered_set>
#include "plants_storage.h"
#include "plant_properties.h"
#include "growth_manager.h"
#include "constrainers.h"

enum ConstrainerType{
    Age,
    Illumination,
    SoilHumidity
};

typedef std::map<ConstrainerType, float> Strengths;

struct Constrainers{
    AgeConstrainer age_constrainer;
    IlluminationConstrainer illumination_constrainer;
    SoilHumidityConstrainer soil_humidity_constrainer;

    Constrainers( AgeConstrainer age_constrainer,
                  IlluminationConstrainer illumination_constrainer,
                  SoilHumidityConstrainer soil_humidity_constrainer):
        age_constrainer(age_constrainer),
        illumination_constrainer(illumination_constrainer),
        soil_humidity_constrainer(soil_humidity_constrainer){}
};

class Plant {
public:
    Plant(const SpecieProperties * m_specie_properties, std::shared_ptr<const QColor> p_color, QPoint p_center_coord, long p_unique_id, int p_random_id);
    ~Plant();

    void newMonth();

    float getHeight() const;
    float getCanopyRadius() const;
    float getRootsRadius() const;
    int getMinimumSoilHumidityRequirement() const;
    int getVigor() const;
    const QColor * getColor() const;

    PlantStatus getStatus();
    void calculateStrength(int p_shaded_percentage, int p_soil_humidity_percentage);

    const int m_unique_id;
    const QPoint m_center_position;
    const QString m_specie_name;
private:
    // Managers
    GrowthManager m_growth_manager;

    // Constrainers
    Constrainers m_constrainers;

    Strengths m_strengths;
    ConstrainerType m_strength_bottleneck;

    std::shared_ptr<const QColor> m_color;
    int m_strength;
    int m_random_id; // Random number between 0 and 1000 used for statistical purposes
    int m_age;

    int m_pain_enducer;
};

#endif //PLANT_H
