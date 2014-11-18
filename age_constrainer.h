#ifndef AGE_CONSTRAINER_H
#define AGE_CONSTRAINER_H
#include "helper.h"
#include <memory>

struct AgeingProperties {
    int probability_of_death_at_birth;
    int probabiity_of_death_at_prime;
    int probability_of_death_at_max;

    int avg_prime_age;
    int avg_max_age;

    AgeingProperties(int p_probability_of_death_at_birth, int p_probabiity_of_death_at_prime, int p_probability_of_death_at_max,
                     int p_avg_prime_age, int p_avg_max_age) : probability_of_death_at_birth(p_probability_of_death_at_birth),
        probabiity_of_death_at_prime(p_probabiity_of_death_at_prime), probability_of_death_at_max(p_probability_of_death_at_max),
        avg_prime_age(p_avg_prime_age), avg_max_age(p_avg_max_age) {}
};

class AgeConstrainer
{
public:
    AgeConstrainer(std::shared_ptr<AgeingProperties> p_ageing_properties);
    ~AgeConstrainer();

    void incrementAge(int months = 1);
    int getStrength();
    int getAge() { return m_age; }

private:
    int m_age;
    std::shared_ptr<AgeingProperties> m_ageing_properties;
    LinearEquation * m_pre_prime_equation;
    LinearEquation * m_post_prime_equation;

    void print_equations();
};



#endif // AGE_CONSTRAINER_H
