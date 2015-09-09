#include "dice_roller.h"

DiceRoller::DiceRoller(int from, int to) :
    generator(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())),
    distribution(std::uniform_int_distribution<int>(from,to))
{
}

DiceRoller::~DiceRoller()
{
//    delete generator;
//    delete distribution;
}

int DiceRoller::generate()
{
    return distribution.operator()(generator);
}
