#include "constrainers.h"
#include "math.h"


/*******************
 * AGE CONSTRAINER *
 *******************/

AgeConstrainer::AgeConstrainer(const AgeingProperties * p_ageing_properties) :
    m_age(0), m_properties(*p_ageing_properties)
{
    // Build the pre-prime linear equation
    {
        m_pre_prime_equation.a =  ( ((float)(m_properties.probability_of_death_at_birth + MAX_STRENGTH)) / m_properties.prime_age_start);
        m_pre_prime_equation.b = ( -m_properties.probability_of_death_at_birth );
    }

    // Build the post-prime linear equation
    {
        m_post_prime_equation.a = ( ((float)(m_properties.probability_of_death_at_upper + MAX_STRENGTH)) / (m_properties.upper_age - m_properties.prime_age_end));
        m_post_prime_equation.a *= -1; // Negative slope
        m_post_prime_equation.b = ( (-1.f * m_post_prime_equation.a * m_properties.prime_age_end) + MAX_STRENGTH);
    }
}

AgeConstrainer::~AgeConstrainer()
{
}

void AgeConstrainer::setAge(int p_age_in_months)
{
    m_age = p_age_in_months;
}

int AgeConstrainer::getStrength() const
{
    if(m_age < m_properties.prime_age_start)
    {
        return m_pre_prime_equation.calculateY(m_age);
    }
    else if(m_age > m_properties.prime_age_end)// post prime
    {
        return max((float) MIN_STRENGTH, m_post_prime_equation.calculateY(m_age));
    }

    return MAX_STRENGTH;
}

/****************************
 * ILLUMINATION CONSTRAINER *
 ****************************/
IlluminationConstrainer::IlluminationConstrainer(const IlluminationProperties * p_illumination_properties) :
    m_shaded_percentage(.0f), m_properties(*p_illumination_properties)
{
    // Build the linear equation
    m_equation.a = (MAX_STRENGTH * 2) /
              ((100-m_properties.shade_tolerance)/m_properties.sensitivity);
    m_equation.a *= -1; // Negative slope

    m_equation.b = ( -1 * m_equation.a * m_properties.shade_tolerance + MAX_STRENGTH);
}

IlluminationConstrainer::~IlluminationConstrainer()
{
}

void IlluminationConstrainer::setShadedPercentage(int p_shaded_percentage)
{
    m_shaded_percentage = p_shaded_percentage;
}

/**
 * @brief IlluminationConstrainer::getStrength
 * @param p_ratio --> shadowed / all
 * @return
 */
int IlluminationConstrainer::getStrength() const
{
    if(m_shaded_percentage > m_properties.shade_tolerance )
        return max((float)MIN_STRENGTH, m_equation.calculateY(m_shaded_percentage));

    return MAX_STRENGTH; // Full strength
}

/*****************
 * SOIL HUMIDITY *
 *****************/

SoilHumidityConstrainer::SoilHumidityConstrainer(const SoilHumidityProperties * p_soil_humidity_properties) :
    m_properties(*p_soil_humidity_properties)
{
    // Build the drought equation
    if(p_soil_humidity_properties->soil_humidity_percentage_prime_start > 0)
    {
        m_drought_equation.a = ((MAX_STRENGTH*2) / (((float)m_properties.soil_humidity_percentage_prime_start)/m_properties.sensitivity));
        m_drought_equation.b = -m_drought_equation.a * m_properties.soil_humidity_percentage_prime_start + 100;
    }
    else
    {
        m_flood_equation.a  = 0;
        m_flood_equation.b = MAX_STRENGTH;
    }

    // Build the flooding equation
    if(p_soil_humidity_properties->soil_humidity_percentage_prime_end < 100)
    {
        m_flood_equation.a = ((-2 * MAX_STRENGTH) / ((100-m_properties.soil_humidity_percentage_prime_end)/m_properties.sensitivity));
        m_flood_equation.b = ( -1 * m_flood_equation.a * m_properties.soil_humidity_percentage_prime_end + 100 );
    }
    else
    {
        m_flood_equation.a  = 0;
        m_flood_equation.b = MAX_STRENGTH;
    }
}

SoilHumidityConstrainer::~SoilHumidityConstrainer()
{

}

int SoilHumidityConstrainer::getStrength() const
{
    if(m_soil_humidity < m_properties.soil_humidity_percentage_prime_start)
        return max(MIN_STRENGTH,(int) m_drought_equation.calculateY(m_soil_humidity));
    else if(m_soil_humidity > m_properties.soil_humidity_percentage_prime_end)
        return max(MIN_STRENGTH, (int) m_flood_equation.calculateY(m_soil_humidity));

    // Else return maximum strength
    return MAX_STRENGTH;
}

bool SoilHumidityConstrainer::isInDrought() const
{
    return m_soil_humidity < m_properties.soil_humidity_percentage_prime_start;
}

void SoilHumidityConstrainer::setSoilHumidityPercentage(int p_soil_humidity_percentage)
{
    m_soil_humidity = p_soil_humidity_percentage;
}

int SoilHumidityConstrainer::getMinimumPrimeSoilHumidity() const
{
    return m_properties.soil_humidity_percentage_prime_start;
}

/***************
 * TEMPERATURE *
 ***************/
#define DEFAULT_DRECREASE_RANGE 15
TemperatureConstrainer::TemperatureConstrainer(const TemperatureProperties * p_temperature_properties) :
    m_properties(*p_temperature_properties)
{
    // Build coldness equation
    m_coldness_equation.a = ((MAX_STRENGTH*2) / (DEFAULT_DRECREASE_RANGE/m_properties.sensitivity));
    m_coldness_equation.b = -m_coldness_equation.a * m_properties.temperature_range_start + MAX_STRENGTH;

    // Build the heat equation
    m_coldness_equation.a = ((MAX_STRENGTH*2) / (DEFAULT_DRECREASE_RANGE/m_properties.sensitivity));
    m_coldness_equation.b = -m_coldness_equation.a * m_properties.temperature_range_start + MAX_STRENGTH;
}

TemperatureConstrainer::~TemperatureConstrainer()
{

}

int TemperatureConstrainer::getStrength() const
{

}

void TemperatureConstrainer::setGroundTemperature(int p_ground_temperature)
{

}
