#include "constrainers.h"
#include "math.h"


/*******************
 * AGE CONSTRAINER *
 *******************/

AgeConstrainer::AgeConstrainer(std::shared_ptr<AgeingProperties> p_ageing_properties) :
    m_age(0), m_prime_age_start(p_ageing_properties->prime_age_start),
    m_prime_age_end(p_ageing_properties->prime_age_end),
    m_pre_prime_equation(NULL), m_post_prime_equation(NULL)
{
    // Build the pre-prime linear equation
    {
        float a ( ((float)(p_ageing_properties->probability_of_death_at_birth + MAX_STRENGTH)) / p_ageing_properties->prime_age_start);
        float b ( -p_ageing_properties->probability_of_death_at_birth );
        m_pre_prime_equation = new LinearEquation(a,b);
    }

    // Build the post-prime linear equation
    {
        float a ( ((float)(p_ageing_properties->probability_of_death_at_upper + MAX_STRENGTH)) / (p_ageing_properties->upper_age - p_ageing_properties->prime_age_end));
        a *= -1; // Negative slope
        float b ( (-1.f * a * p_ageing_properties->prime_age_end) + MAX_STRENGTH);
        m_post_prime_equation = new LinearEquation(a,b);
    }

//    std::cout << "Pre prime equation";
//    m_pre_prime_equation->print();

//    std::cout << "Post prime equation";
//    m_post_prime_equation->print();
}

AgeConstrainer::~AgeConstrainer()
{
    delete m_pre_prime_equation;
    delete m_post_prime_equation;
}

void AgeConstrainer::setAge(int p_age_in_months)
{
    m_age = p_age_in_months;
}

int AgeConstrainer::getStrength()
{
    if(m_age < m_prime_age_start)
    {
        return m_pre_prime_equation->calculateY(m_age);
    }
    else if(m_age > m_prime_age_end)// post prime
    {
        return max((float) MIN_STRENGTH, m_post_prime_equation->calculateY(m_age));
    }

    return MAX_STRENGTH;
}

/****************************
 * ILLUMINATION CONSTRAINER *
 ****************************/
IlluminationConstrainer::IlluminationConstrainer(std::shared_ptr<IlluminationProperties> p_illumination_properties) :
    m_shaded_percentage(.0f),
    m_ratio_start_of_negative_impact(p_illumination_properties->shadowed_percentage_start_of_negative_impact)
{
    // Build the linear equation
    float a ((MAX_STRENGTH + p_illumination_properties->probability_of_death_at_max_shade) /
              (p_illumination_properties->max_shadowed_percentage -m_ratio_start_of_negative_impact));
    a *= -1; // Negative slope

    float b ( (-1.0f * a * m_ratio_start_of_negative_impact) + MAX_STRENGTH);

    m_equation = new LinearEquation(a,b);

//    std::cout << "Lighting equation";
//    m_equation->print();
}

IlluminationConstrainer::~IlluminationConstrainer()
{
    delete m_equation;
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
int IlluminationConstrainer::getStrength()
{
    if(m_shaded_percentage > m_ratio_start_of_negative_impact )
        return max((float)MIN_STRENGTH, m_equation->calculateY(m_shaded_percentage));

    return MAX_STRENGTH; // Full strength
}

/*****************
 * SOIL HUMIDITY *
 *****************/

SoilHumidityConstrainer::SoilHumidityConstrainer(std::shared_ptr<SoilHumidityProperties> p_humidity_properties) :
    m_soil_humidity_prime_start_percentage (p_humidity_properties->soil_humidity_percentage_prime_start),
    m_soil_humidity_prime_end_percentage (p_humidity_properties->soil_humidity_percentage_prime_end)
{
    // Build the drought equation
    {
        float a ((MAX_STRENGTH*2) / m_soil_humidity_prime_start_percentage);
        float b (MIN_STRENGTH);
        m_drought_equation = new LinearEquation(a,b);
    }

    // Build the flooding equation
    {
        float a ((-2 * MAX_STRENGTH) / (100-m_soil_humidity_prime_end_percentage));
        float b ( -1 * a * m_soil_humidity_prime_end_percentage + 100 );
        m_flood_equation = new LinearEquation(a,b);
    }
}

SoilHumidityConstrainer::~SoilHumidityConstrainer()
{
    delete m_drought_equation;
    delete m_flood_equation;
}

int SoilHumidityConstrainer::getStrength()
{
    if(m_soil_humidity < m_soil_humidity_prime_start_percentage)
        return m_drought_equation->calculateY(m_soil_humidity);
    else if(m_soil_humidity > m_soil_humidity_prime_end_percentage)
        return m_flood_equation->calculateY(m_soil_humidity);

    // Else return maximum strength
    return MAX_STRENGTH;
}

bool SoilHumidityConstrainer::isInDrought()
{
    return m_soil_humidity < m_soil_humidity_prime_start_percentage;
}

void SoilHumidityConstrainer::setSoilHumidityPercentage(int p_soil_humidity_percentage)
{
    m_soil_humidity = p_soil_humidity_percentage;
}
