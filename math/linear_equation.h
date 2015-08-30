#ifndef HELPER_H
#define HELPER_H

#include "boost/functional/hash.hpp"

class LinearEquation
{
public:
    LinearEquation();
    LinearEquation(double p_a, double p_b);
    float calculateY(float x) const;
    void print();

    float a;
    float b;
};

#endif // HELPER_H
