#include "plant.h"
#include "boost/foreach.hpp"

#include "growth_manager.h"

#include "constants.h"

#include "debuger.h"
#include "constrainers.h"

Plant::Plant(std::string p_name, QColor p_color, QPoint p_center_coord, long p_unique_id, int p_random_id,
      std::shared_ptr<GrowthProperties>  p_growth_properties, std::shared_ptr<AgeingProperties> p_ageing_properties,
      std::shared_ptr<IlluminationProperties> p_illumination_properties, std::shared_ptr<SoilHumidityProperties> p_soil_himidity_properties) :
    m_name(p_name), m_color(p_color), m_center_position(p_center_coord), m_unique_id(p_unique_id), m_random_id(p_random_id), m_strengths(), m_age(0),
    m_growth_manager(new GrowthManager(p_growth_properties)), m_min_strength(MAX_STRENGTH)
{
    m_constrainers.age_constrainer = new AgeConstrainer(p_ageing_properties);
    m_constrainers.illumination_constrainer = new IlluminationConstrainer(p_illumination_properties);
    m_constrainers.soil_humidity_constrainer = new SoilHumidityConstrainer(p_soil_himidity_properties);

    m_strengths.insert(std::pair<ConstrainerType, int>(ConstrainerType::Age, -100));
    m_strengths.insert(std::pair<ConstrainerType, int>(ConstrainerType::Illumination, -100));
    m_strengths.insert(std::pair<ConstrainerType, int>(ConstrainerType::SoilHumidity, -100));
}

Plant::~Plant()
{
    delete m_growth_manager;

    // Delete the constrainers
    delete m_constrainers.age_constrainer;
    delete m_constrainers.illumination_constrainer;
    delete m_constrainers.soil_humidity_constrainer;
}

void Plant::newMonth()
{
    m_age++;

    // Age constrainer
    m_constrainers.age_constrainer->setAge(m_age);

    // Grow
    if(m_min_strength > 0) // Only grow if resource balance is positif
        m_growth_manager->grow(m_min_strength); // TODO: Replace with calculated strength
}

void Plant::calculateStrength(int p_shaded_percentage, int p_soil_humidity_percentage) // Must be called before newMonth is triggered
{
    /*******
     * AGE *
     *******/
    m_constrainers.age_constrainer->setAge(m_age);
    int age_strength (m_constrainers.age_constrainer->getStrength());
    m_strengths.find(ConstrainerType::Age)->second = age_strength;
    int min_strength (age_strength);
    ConstrainerType bottleneck(ConstrainerType::Age);

    /****************
     * ILLUMINATION *
     ****************/
    m_constrainers.illumination_constrainer->setShadedPercentage(p_shaded_percentage);
    int illumination_strength (m_constrainers.illumination_constrainer->getStrength());
    m_strengths.find(ConstrainerType::Illumination)->second = illumination_strength;
    if(illumination_strength < min_strength)
    {
        min_strength = illumination_strength;
        bottleneck = ConstrainerType::Illumination;
    }


    /*****************
     * SOIL HUMIDITY *
     *****************/
    m_constrainers.soil_humidity_constrainer->setSoilHumidityPercentage(p_soil_humidity_percentage);
    int soil_humidity_strength (m_constrainers.soil_humidity_constrainer->getStrength());
    m_strengths.find(ConstrainerType::SoilHumidity)->second = soil_humidity_strength;
    if(soil_humidity_strength < min_strength)
    {
        min_strength = soil_humidity_strength;
        bottleneck = ConstrainerType::SoilHumidity;
    }
    std::cout << "Soil humidity: " << p_soil_humidity_percentage <<
                 " | Soil humidity range: [" << m_constrainers.soil_humidity_constrainer->m_soil_humidity_prime_start_percentage <<
                                        " - " << m_constrainers.soil_humidity_constrainer->m_soil_humidity_prime_end_percentage  << "] " <<
                 " | Calculated stength: " << soil_humidity_strength << std::endl;

    m_min_strength = min_strength;
    m_strength_bottleneck = bottleneck;
}

float Plant::getHeight() const
{
    return m_growth_manager->getHeight();
}

float Plant::getCanopyRadius() const
{
    return m_growth_manager->getCanopyRadius();
}

float Plant::getRootsRadius() const
{
    return m_growth_manager->getRootsRadius();
}

int Plant::getMinimumSoilHumidityRequirement() const
{
    return m_constrainers.soil_humidity_constrainer->m_soil_humidity_prime_start_percentage;
}

int Plant::getVigor() const
{
    return m_min_strength;
}

PlantStatus Plant::getStatus()
{
    if(m_min_strength < 0 && m_random_id < (m_min_strength * -1.f * 10)) // Could die
    {
        switch(m_strength_bottleneck){
        case Illumination:
            return DeathByIllumination;
        case Age:
            return DeathByAge;
        case SoilHumidity:
            if (m_constrainers.soil_humidity_constrainer->isInDrought())
                return DeathByDrought;
            else
                return DeathByFlood;
        }
    }
    return Alive;
}
