#include "plant.h"
#include "boost/foreach.hpp"
#include "growth_manager.h"
#include <math.h>
#include "../../utils/utils.h"

#include <QDebug>
Plant::Plant(SpecieProperties p_specie_properties, QColor p_color, QPoint p_center_coord, long p_unique_id, int p_random_id) :
    m_center_position(p_center_coord), m_unique_id(p_unique_id), m_random_id(p_random_id), m_strengths(), m_age(0), m_strength(Constrainer::_MAX_STRENGTH),
    m_color(p_color),m_specie_name(p_specie_properties.specie_name), m_specie_id(p_specie_properties.specie_id),
    m_constrainers(AgeConstrainer(p_specie_properties.ageing_properties),
                   IlluminationConstrainer(p_specie_properties.illumination_properties),
                   SoilHumidityConstrainer(p_specie_properties.soil_humidity_properties),
                   TemperatureConstrainer(p_specie_properties.temperature_properties)),
    m_seeding_properties(p_specie_properties.seeding_properties),
    m_growth_manager(GrowthManager(p_specie_properties.growth_properties, p_specie_properties.ageing_properties)),
    m_pain_enducer(0)
{
    m_strengths.emplace(ConstrainerType::Age, Constrainer::_MIN_STRENGTH);
    m_strengths.emplace(ConstrainerType::Illumination, Constrainer::_MIN_STRENGTH);
    m_strengths.emplace(ConstrainerType::SoilHumidity, Constrainer::_MIN_STRENGTH);
    m_strengths.emplace(ConstrainerType::Temperature, Constrainer::_MIN_STRENGTH);
}

Plant::~Plant()
{
}

//Plant & Plant::operator=(const Plant& other)
//{
//    if (this != &other) // protect against invalid self-assignment
//    {
//        m_unique_id = other.m_unique_id;
//        m_center_position = other.m_center_position;
//        m_specie_name = other.m_specie_name;
//        m_specie_id = other.m_specie_id;
//        m_color = other.m_color;
//        m_growth_manager = other.m_growth_manager;
//        m_seeding_properties = other.m_seeding_properties;
//        m_constrainers = other.m_constrainers;
//        m_strengths = other.m_strengths;
//        m_strength_bottleneck = other.m_strength_bottleneck;
//        m_strength = other.m_strength;
//        m_random_id = other.m_random_id;
//        m_age = other.m_age;
//        m_pain_enducer = other.m_pain_enducer;
//    }
//    // by convention, always return *this
//    return *this;
//}

void Plant::newMonth()
{
    m_age++;

    // Age constrainer
    m_constrainers.age_constrainer.setAge(m_age);

    // Grow
    if(m_strength > 0) // Only grow if resource balance is positif
        m_growth_manager.grow(m_strength); // TODO: Replace with calculated strength
}

void Plant::calculateStrength(int p_daily_illumination, int p_soil_humidity_percentage, int p_temp) // Must be called before newMonth is triggered
{
    /*******
     * AGE *
     *******/
    m_constrainers.age_constrainer.setAge(m_age);
    int age_strength (m_constrainers.age_constrainer.getStrength());
    m_strengths.find(ConstrainerType::Age)->second = age_strength;
    int min_strength (age_strength);
    ConstrainerType bottleneck(ConstrainerType::Age);

    /****************
     * ILLUMINATION *
     ****************/
    m_constrainers.illumination_constrainer.setDailyIllumination(p_daily_illumination);
    int illumination_strength (m_constrainers.illumination_constrainer.getStrength());
    m_strengths.find(ConstrainerType::Illumination)->second = illumination_strength;
    if(illumination_strength < min_strength)
    {
        min_strength = illumination_strength;
        bottleneck = ConstrainerType::Illumination;
    }

    /*****************
     * SOIL HUMIDITY *
     *****************/
    m_constrainers.soil_humidity_constrainer.setSoilHumidity(p_soil_humidity_percentage);
    int soil_humidity_strength (m_constrainers.soil_humidity_constrainer.getStrength());
    m_strengths.find(ConstrainerType::SoilHumidity)->second = soil_humidity_strength;
    if(soil_humidity_strength < min_strength)
    {
        min_strength = soil_humidity_strength;
        bottleneck = ConstrainerType::SoilHumidity;
    }

    /***************
     * TEMPERATURE *
     ***************/
    m_constrainers.temp_constrainer.setTemperature(p_temp);
    int temp_strength = m_constrainers.temp_constrainer.getStrength();
    m_strengths.find(ConstrainerType::Temperature)->second = temp_strength;
    if(temp_strength < min_strength)
    {
        min_strength = temp_strength;
        bottleneck = ConstrainerType::Temperature;
    }

    // Pain enducer is used to prevent a plant from being in negative strength too long
    if(min_strength < 0)
        m_pain_enducer += 10;
    else
        m_pain_enducer = 0;

    m_strength = min_strength-m_pain_enducer;

    m_strength_bottleneck = bottleneck;
}

float Plant::getHeight() const
{
    return m_growth_manager.getHeight();
}

float Plant::getCanopyWidth() const
{
    return m_growth_manager.getCanopyWidth();
}

float Plant::getRootSize() const
{
    return m_growth_manager.getRootSize();
}

int Plant::getMinimumSoilHumidityRequirement() const
{
    return m_constrainers.soil_humidity_constrainer.getMinimumPrimeSoilHumidity();
}

int Plant::getVigor() const
{
    return m_strength;
}

std::vector<QPoint> Plant::seed()
{
    // Number of seeds proportianal to strength
    int seed_count((int) ((((float)m_strength)/Constrainer::_MAX_STRENGTH) * m_seeding_properties.seed_count));

    return seed(seed_count);
}

std::vector<QPoint> Plant::seed(int seed_count)
{
    std::vector<QPoint> seeds;
    int max_distance(m_seeding_properties.max_seed_distance * 100); // To centimeters

    for( int i(0); i < seed_count; i++ )
        seeds.push_back(Utils::getRandomPointInCircle(m_center_position, max_distance));

    return seeds;
}

Plant::PlantStatus Plant::getStatus()
{
    if(m_strength < 0 && m_random_id <= (m_strength * -1.f * 10)) // Die
    {
        switch(m_strength_bottleneck){
        case Illumination:
            if(m_constrainers.illumination_constrainer.isUnderExposed())
                return DeathByUnderIllumination;
            else
                return DeathByOverIllumination;
        case Age:
            return DeathByAge;
        case SoilHumidity:
            if (m_constrainers.soil_humidity_constrainer.isInDrought())
                return DeathByDrought;
            else
                return DeathByFlood;
        case Temperature:
            if(m_constrainers.temp_constrainer.isTooCold())
                return DeathByCold;
            else
                return DeathByHeat;
        }
    }
    return Alive;
}

QColor Plant::getColor() const
{
    return m_color;
}
