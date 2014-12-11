#ifndef HELPER_H
#define HELPER_H

#include "boost/functional/hash.hpp"
#include <map>
#include <set>

class Plant;

using namespace std;

#include <QPoint>

enum PlantStatus{
    Alive,
    DeathByAge,
    DeathByIllumination,
    DeathByDrought,
    DeathByFlood
};

//struct Coordinate{
//public:
//    Coordinate() : x(0), y(0) {}
//    Coordinate(int x, int y) : x(x), y(y) {}
//    Coordinate(const Coordinate & other) : x(other.x), y(other.y) {}

//    int x,y;
//};

struct LinearEquation
{
public:
    float a;
    float b;


    LinearEquation() : a(.0), b(.0) {
    }

    LinearEquation(double p_a, double p_b) : a(p_a), b(p_b) {
    }

    float calculateY(float x) const
    {
        return a * x + b;
    }

    void print()
    {
        std::cout << a << " X + " << b << std::endl;
    }
};

namespace std {
  template <>
  struct hash<QPoint>
  {
    std::size_t operator()(const QPoint & t) const
    {
      std::size_t val = 0 ;
      boost::hash_combine(val,t.x());
      boost::hash_combine(val,t.y());
      return val;
    }
  };
}

#endif // HELPER_H
