#include "helper.h"
#include <algorithm>

extern bool operator==(Coordinate const& c1, Coordinate const& c2)
{
   return c1.x == c1.x && c2.y == c2.y;
}

/*
 * Merges set values from c2 into c1
 */
extern void merge_coordinate_holders(CoordinateHolder & c1, const CoordinateHolder & c2)
{
    for(CoordinateHolder::const_iterator it = c2.begin(); it != c2.end(); it++)
    {
        c1[it->first].insert(it->second.begin(), it->second.end());
    }
}
