#include "environment_soil_humidity.h"
#include <math.h>
#include "../data_holders/pixel_data.h"

EnvironmentSoilHumidity::EnvironmentSoilHumidity()
{

}

int EnvironmentSoilHumidity::getSoilHumidity(EnvironmentSpatialHashMap & map, QPoint p_center, float p_roots_size, int p_id)
{
    std::vector<QPoint> cells(map.getPoints(p_center, p_roots_size));

    int aggregated_humidity_percentage(0);

    for(QPoint & cell : cells)
    {
        aggregated_humidity_percentage += map[cell].soil_humidity_cell.getGrantedHumidity(p_id);
    }
    return aggregated_humidity_percentage/cells.size(); // Return percentage
}

// DO NOT CALL THIS METHOD FOLLOWED BY GETHUMIDITY CONTRINUOUSLY RATHER UPDATE THIS FOR ALL NECESSARY CELLS IN ONE GO
void EnvironmentSoilHumidity::update(EnvironmentSpatialHashMap & map, QPoint p_center, float p_roots_size, int p_id, int p_minimum_humidity)
{
    std::vector<QPoint> cells(map.getPoints(p_center, p_roots_size));

    for(QPoint & cell : cells)
    {
        map[cell].soil_humidity_cell.update(p_id, p_roots_size, p_minimum_humidity);
    }
}

void EnvironmentSoilHumidity::remove(EnvironmentSpatialHashMap & map, QPoint p_center, float p_roots_size, int p_id)
{
    std::vector<QPoint> cells(map.getPoints(p_center, p_roots_size));

    for(QPoint & cell : cells)
    {
        map[cell].soil_humidity_cell.remove(p_id);
    }
}

//std::pair<int,int> EnvironmentSoilHumidity::getRange()
//{
//    return m_range;
//}
