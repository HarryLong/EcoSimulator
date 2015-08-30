#include "linear_equation.h"
#include <iostream>

LinearEquation::LinearEquation() : a(.0), b(.0)
{
}

LinearEquation::LinearEquation(double p_a, double p_b) : a(p_a), b(p_b)
{
}

float LinearEquation::calculateY(float x) const
{
    return a * x + b;
}

void LinearEquation::print()
{
    std::cout << a << " X + " << b << std::endl;
}
