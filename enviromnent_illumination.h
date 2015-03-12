#ifndef ENVIRONMNENT_ILLUMINATION_H
#define ENVIRONMNENT_ILLUMINATION_H

#include <QPoint>
#include <QImage>
#include <unordered_map>
#include <memory>
#include "environment_spatial_hashmap.h"
#include "input_widgets.h"

class EnvironmentIllumination
{
public:
    EnvironmentIllumination(EnvironmentSpatialHashMap & map);

    void setIlluminationData(PixelData * p_data);
    int getDailyIllumination(QPoint p_center, float p_canopy_width, float p_height);
    void setData(QPoint p_center, float p_canopy_width, float height, int p_id);
    void remove(QPoint p_center, float p_canopy_width, int p_id);
    float getMaxHeight(QPoint p_cell_coord);

protected:

private:
    EnvironmentSpatialHashMap& m_map;
};

#endif //ENVIRONMNENT_ILLUMINATION_H
