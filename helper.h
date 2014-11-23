#ifndef HELPER_H
#define HELPER_H

#include "boost/functional/hash.hpp"
#include <map>
#include <set>

class Plant;

using namespace std;

enum PlantStatus{
    Alive,
    DeathByAge,
    DeathByIllumination
};

struct Coordinate{
public:
    Coordinate() : x(0), y(0) {}
    Coordinate(int x, int y) : x(x), y(y) {}
    Coordinate(const Coordinate & other) : x(other.x), y(other.y) {}

    int x,y;
};

struct LinearEquation
{
public:
    float a;
    float b;

    LinearEquation(double p_a, double p_b) : a(p_a), b(p_b) {
    }

    float calculateY(float x)
    {
        return a * x + b;
    }
};

namespace std {
  template <>
  struct hash<Coordinate>
  {
    std::size_t operator()(const Coordinate & t) const
    {
      std::size_t val = 0 ;
      boost::hash_combine(val,t.x);
      boost::hash_combine(val,t.y);
      return val;
    }
  };
}

extern bool operator==(Coordinate const& c1, Coordinate const& c2);

typedef std::map<int, std::set<int> > CoordinateHolder; // X , set of y's

extern void merge_coordinate_holders(CoordinateHolder & c1, const CoordinateHolder & c2);

#endif // HELPER_H
