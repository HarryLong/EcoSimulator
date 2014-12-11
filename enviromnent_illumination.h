#ifndef ENVIRONMNENT_ILLUMINATION_H
#define ENVIRONMNENT_ILLUMINATION_H

#include <QPoint>
#include <QImage>
#include <unordered_map>
#include <memory>
#include "environment_spatial_hashmap.h"

class EnvironmentIllumination
{
public:
    EnvironmentIllumination(EnvironmentSpatialHashMap & map);

    void setIlluminationData(const QImage & p_image);
    int getShadedPercentage(QPoint p_center, float p_canopy_radius, float p_height);
    void setData(QPoint p_center, float p_canopy_radius, float height, int p_id);
    void remove(QPoint p_center, float p_canopy_radius, int p_id);
    float getMaxHeight(QPoint p_cell_coord);

protected:

private:
    EnvironmentSpatialHashMap& m_map;
};

#endif //ENVIRONMNENT_ILLUMINATION_H
