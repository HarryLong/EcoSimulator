#ifndef ENVIRONMNENT_ILLUMINATION_H
#define ENVIRONMNENT_ILLUMINATION_H

#include <QPoint>
#include <QImage>
#include <unordered_map>
#include <memory>

#include "../data_holders/environment_spatial_hashmap.h"

class EnvironmentIllumination
{
public:
    static const int _MIN = 0;
    static const int _MAX = 24;

    EnvironmentIllumination();

    int getDailyIllumination(EnvironmentSpatialHashMap & map, QPoint p_center, int p_id, float p_canopy_width, float height);
    void update(EnvironmentSpatialHashMap & map, QPoint p_center, float p_canopy_width, float height, int p_id);
    void remove(EnvironmentSpatialHashMap & map, QPoint p_center, float p_canopy_width, int p_id);
//    float getMaxHeight(QPoint p_cell_coord);

protected:

private:
};

#endif //ENVIRONMNENT_ILLUMINATION_H
