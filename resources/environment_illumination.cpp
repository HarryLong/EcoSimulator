#include "environment_illumination.h"
#include <QImage>
#include <math.h>

#include "../data_holders/pixel_data.h"

#include <QDebug>

EnvironmentIllumination::EnvironmentIllumination()
{

}

//float EnvironmentIllumination::getMaxHeight(QPoint p_cell_coord)
//{
//    return m_map.get(p_cell_coord)->illumination_cell->m_max_height;
//}

#define HEIGHT_BUFFER 5 //cm
int EnvironmentIllumination::getDailyIllumination(EnvironmentSpatialHashMap & map, QPoint p_center, int p_id, float p_canopy_width, float height)
{
    std::vector<QPoint> cells(map.getPoints(p_center, p_canopy_width/2));

    float aggregated_daily_illumination(0);

    for(QPoint & cell : cells)
    {
        aggregated_daily_illumination += map.getCell(cell, EnvironmentSpatialHashMap::Space::_HASHMAP).illumination_cell.getIllumination(p_id, height);
    }

    return std::round(aggregated_daily_illumination/cells.size()); // Divide by cells iterated over
}

void EnvironmentIllumination::update(EnvironmentSpatialHashMap & map, QPoint p_center, float p_canopy_width, float p_height, int p_id)
{
    std::vector<QPoint> cells(map.getPoints(p_center, p_canopy_width/2));

    for(QPoint & cell : cells)
    {
        map.getCell(cell, EnvironmentSpatialHashMap::Space::_HASHMAP).illumination_cell.update(p_id, p_height);
    }
}

void EnvironmentIllumination::remove(EnvironmentSpatialHashMap & map, QPoint p_center, float p_canopy_width, int p_id)
{
    std::vector<QPoint> cells(map.getPoints(p_center, p_canopy_width/2));

    for(QPoint & cell : cells)
    {
        map.getCell(cell, EnvironmentSpatialHashMap::Space::_HASHMAP).illumination_cell.remove(p_id);
    }
}
