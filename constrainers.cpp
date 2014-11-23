#include "constrainers.h"
#include "math.h"


/*******************
 * AGE CONSTRAINER *
 *******************/

AgeConstrainer::AgeConstrainer(std::shared_ptr<AgeingProperties> p_ageing_properties) : m_age(0), m_pre_prime_equation(NULL), m_post_prime_equation(NULL)
{
    // Build the pre-prime linear equation
    {
        float a ( (p_ageing_properties->probability_of_death_at_birth + 1.0f) / p_ageing_properties->prime_age);
        float b ( -p_ageing_properties->prime_age );
        m_pre_prime_equation = new LinearEquation(a,b);
    }

    // Build the post-prime linear equation
    {
        double a ( (1.0f + p_ageing_properties->probability_of_death_at_upper) / (p_ageing_properties->upper_age - p_ageing_properties->prime_age));
        a *= -1; // Negative slope
        double b ( (-1.f * a * p_ageing_properties->prime_age) + 1.0f);
        m_post_prime_equation = new LinearEquation(a,b);
    }
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

float AgeConstrainer::getStrength()
{
    if(m_age <= m_prime_age)
    {
        return m_pre_prime_equation->calculateY(m_age);
    }
    else // post prime
    {
        return max(-1.f, m_post_prime_equation->calculateY(m_age));
    }
}

/****************************
 * ILLUMINATION CONSTRAINER *
 ****************************/

IlluminationConstrainer::IlluminationConstrainer(std::shared_ptr<IlluminationProperties> p_illumination_properties) :
    m_shaded_ratio(.0f)
{
    m_ratio_start_of_negative_impact = p_illumination_properties->ratio_start_of_negative_impact;
    // Build the linear equation
    double a ((1.0f + p_illumination_properties->probability_of_death_at_max_shade) /
              (p_illumination_properties->ratio_max_shade - p_illumination_properties->ratio_start_of_negative_impact));
    a *= -1; // Negative slope

    double b ( (-1.0f * a * p_illumination_properties->ratio_start_of_negative_impact) + 1.0f);

    m_equation = new LinearEquation(a,b);
}

IlluminationConstrainer::~IlluminationConstrainer()
{
    delete m_equation;
}

void IlluminationConstrainer::setShadedRatio(float p_shaded_ratio)
{
    m_shaded_ratio = p_shaded_ratio;
}

/**
 * @brief IlluminationConstrainer::getStrength
 * @param p_ratio --> shadowed / all
 * @return
 */
float IlluminationConstrainer::getStrength()
{
    if(m_shaded_ratio > m_ratio_start_of_negative_impact )
        return max(1.f, m_equation->calculateY(m_shaded_ratio));

    return 1.f; // Full strength
}

