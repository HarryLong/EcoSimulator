#ifndef ENVIRONMNENT_ILLUMINATION_H
#define ENVIRONMNENT_ILLUMINATION_H

#include "spatial_hashmap.h"

struct IlluminationCellContent {
    bool lit;
    float max_height;
    std::unordered_map<int, float> id_to_height;

    IlluminationCellContent() : max_height(.0f), id_to_height(), lit(true) {}
};
typedef SpatialHashMap<IlluminationCellContent> IlluminationSpatialHashMap;
class EnvironmentIllumination
{
public:
    EnvironmentIllumination();

    // Get illumination percentage
    float getIlluminationPercentage(Coordinate p_center, float p_radius, float p_height);
    void setSize(Coordinate p_center, float p_radius, float height, int p_id);
    void remove(Coordinate p_center, float p_radius, int p_id);
    float getMaxHeight(Coordinate p_cell_coord);
    void reset();
    IlluminationSpatialHashMap getIlluminationSpatialHashmap() { return m_spatial_hashmap; }

protected:
    void init();

private:
    IlluminationSpatialHashMap m_spatial_hashmap;
};

#endif //ENVIRONMNENT_ILLUMINATION_H
