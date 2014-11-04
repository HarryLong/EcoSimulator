#include "policies.h"

/*******************
 * AGEING POLICIES *
 *******************/
AgeingPolicy::AgeingPolicy(AgeingProperties p_ageing_properties) : m_ageing_properties(p_ageing_properties)
{
    // Build the pre-prime linear equation
    {
        float a ( (m_ageing_properties.probability_of_death_at_birth-m_ageing_properties.probabiity_of_death_at_prime) / m_ageing_properties.avg_prime_age );
        float b ( m_ageing_properties.probability_of_death_at_birth );
        m_pre_prime_equation = new LinearEquation(a,b);
    }
    // Build the post-prime linear equation
    {
        float a ( (m_ageing_properties.probability_of_death_at_max-m_ageing_properties.probabiity_of_death_at_prime) / (m_ageing_properties.avg_max_age - m_ageing_properties.avg_prime_age) );
        float b ( m_ageing_properties.probabiity_of_death_at_prime - (a*m_ageing_properties.avg_prime_age) );
        m_post_prime_equation = new LinearEquation(a,b);
    }
}

AgeingPolicy::~AgeingPolicy()
{
    delete m_pre_prime_equation;
    delete m_post_prime_equation;
}

float AgeingPolicy::get_probability_of_dying_of_age()
{
    if(m_age <= m_ageing_properties.avg_prime_age)
    {
        return (m_pre_prime_equation->a * m_age) + m_pre_prime_equation->b;
    }
    else if(m_age <= m_ageing_properties.avg_max_age)
    {
        return (m_post_prime_equation->a * m_age) + m_post_prime_equation->b;
    }
    else // Passed maximum age
    {
        return m_ageing_properties.probability_of_death_at_max;
    }
}

/*******************
 * GROWTH POLICIES *
 *******************/
GrowthPolicy::GrowthPolicy(GrowthProperties p_growth_properties) : m_growth_properties(p_growth_properties), m_height(.0f)
{

}

void GrowthPolicy::grow(float p_strength)
{
    m_height +=  p_strength * m_growth_properties.monthly_factor;
}
