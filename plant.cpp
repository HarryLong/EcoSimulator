#include "plant.h"
#include "boost/foreach.hpp"

#include "growth_manager.h"

#include "constants.h"

#include "debuger.h"
#include "constrainers.h"

Plant::Plant(std::string p_name, QColor p_color, Coordinate p_center_coord, long p_unique_id, int p_random_id,
      std::shared_ptr<GrowthProperties>  p_growth_properties, std::shared_ptr<AgeingProperties> p_ageing_properties,
      std::shared_ptr<IlluminationProperties> p_illumination_properties) :
    m_name(p_name), m_color(p_color), m_center_position(p_center_coord), m_unique_id(p_unique_id), m_random_id(p_random_id), m_strengths(), m_age(0),
    m_growth_manager(new GrowthManager(p_growth_properties))
{
    m_constrainers.age_constrainer = new AgeConstrainer(p_ageing_properties);
    m_constrainers.illumination_constrainer = new IlluminationConstrainer(p_illumination_properties);

    m_strengths.insert(std::pair<ConstrainerType, int>(ConstrainerType::Age, -1));
    m_strengths.insert(std::pair<ConstrainerType, int>(ConstrainerType::Illumination, -1));
}

Plant::~Plant()
{
    delete m_growth_manager;

    // Delete the constrainers
    delete m_constrainers.age_constrainer;
    delete m_constrainers.illumination_constrainer;
}

void Plant::newMonth()
{
    m_age++;

    // Age constrainer
    m_constrainers.age_constrainer->setAge(m_age);

    // Grow
    m_growth_manager->grow(m_min_strength); // TODO: Replace with calculated strength
}

void Plant::calculateStrength(float p_shaded_ratio) // Must be called before newMonth is triggered
{
    /*******
     * Age *
     *******/
    m_constrainers.age_constrainer->setAge(m_age);
    float age_strength (m_constrainers.age_constrainer->getStrength());
    m_strengths.find(ConstrainerType::Age)->second = age_strength;
    float min_strength (age_strength);
    ConstrainerType bottleneck(ConstrainerType::Age);

    /****************
     * Illumination *
     ****************/
    m_constrainers.illumination_constrainer->setShadedRatio(p_shaded_ratio);
    float illumination_strength (m_constrainers.illumination_constrainer->getStrength());
    m_strengths.find(ConstrainerType::Illumination)->second = illumination_strength;
    if(illumination_strength < min_strength)
    {
        min_strength = illumination_strength;
        bottleneck = ConstrainerType::Illumination;
    }
}

float Plant::getHeight() const
{
    return m_growth_manager->getHeight();
}

float Plant::getCanopyRadius() const
{
    return m_growth_manager->getCanopyRadius();
}

PlantStatus Plant::getStatus()
{
    if(m_min_strength < .0f && m_random_id < (m_min_strength * -1.f * 1000)) // Could die
    {
        switch(m_strength_bottleneck){
        case Illumination:
            return DeathByIllumination;
        case Age:
            return DeathByAge;
        }
    }
    return Alive;
}
