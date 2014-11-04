#include "age_constrainer.h"

AgeConstrainer::AgeConstrainer(const AgeingProperties * p_ageing_properties) : m_ageing_properties(p_ageing_properties), m_age(0), m_pre_prime_equation(NULL),
    m_post_prime_equation(NULL)
{
    // Build the pre-prime linear equation
    {
        double a ( -((double)(m_ageing_properties->probability_of_death_at_birth-m_ageing_properties->probabiity_of_death_at_prime) / m_ageing_properties->avg_prime_age ));
        double b ( m_ageing_properties->probability_of_death_at_birth );
        m_pre_prime_equation = new LinearEquation(a,b);
    }
    // Build the post-prime linear equation
    {
        double a ( (double)(m_ageing_properties->probability_of_death_at_max-m_ageing_properties->probabiity_of_death_at_prime) / (m_ageing_properties->avg_max_age - m_ageing_properties->avg_prime_age) );
        double b ( m_ageing_properties->probabiity_of_death_at_prime - (a*m_ageing_properties->avg_prime_age) );
        m_post_prime_equation = new LinearEquation(a,b);
    }

    print_equations();
}

AgeConstrainer::~AgeConstrainer()
{
    delete m_pre_prime_equation;
    delete m_post_prime_equation;
}

void AgeConstrainer::incrementAge(int months)
{
    m_age += months;
}

int AgeConstrainer::getStrength()
{
    if(m_age <= m_ageing_properties->avg_prime_age)
    {
        return 1000 - ((m_pre_prime_equation->a * m_age) + m_pre_prime_equation->b);
    }
    else if(m_age <= m_ageing_properties->avg_max_age)
    {
        return 1000 - ((m_post_prime_equation->a * m_age) + m_post_prime_equation->b);
    }
    else // Passed maximum age
    {
        return 1000 - (m_ageing_properties->probability_of_death_at_max);
    }
}

void AgeConstrainer::print_equations()
{
    std::cout << " Ageing equation from [0, " << m_ageing_properties->avg_prime_age << "] --> " << m_pre_prime_equation->a  << " x + " << m_pre_prime_equation->b << std::endl;
    std::cout << " Ageing equation from [" << m_ageing_properties->avg_prime_age << ", " << m_ageing_properties->avg_max_age <<
                 "] --> " << m_post_prime_equation->a  << " x + " << m_post_prime_equation->b << std::endl;
}
