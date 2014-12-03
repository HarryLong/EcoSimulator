#ifndef ENVIRONMNENT_ILLUMINATION_H
#define ENVIRONMNENT_ILLUMINATION_H

#include "spatial_hashmap.h"
#include <QPoint>
#include <QImage>

struct IlluminationCellContent {
    bool lit;
    float max_height;
    std::unordered_map<int, float> id_to_height;

    IlluminationCellContent(bool lit) : max_height(.0f), id_to_height(), lit(lit) {}
};

typedef SpatialHashMap<IlluminationCellContent> IlluminationSpatialHashMap;
class EnvironmentIllumination
{
public:
    EnvironmentIllumination();

    void setIlluminationData(const QImage* p_image);
    int getShadedPercentage(QPoint p_center, float p_diameter, float p_height);
    void setData(QPoint p_center, float p_canopy_radius, float height, int p_id);
    void remove(QPoint p_center, float p_canopy_radius, int p_id);
    float getMaxHeight(QPoint p_cell_coord);
    void reset();
    IlluminationSpatialHashMap getIlluminationSpatialHashmap() { return m_spatial_hashmap; }

protected:

private:
    IlluminationSpatialHashMap m_spatial_hashmap;

    bool data_set;
};

#endif //ENVIRONMNENT_ILLUMINATION_H
