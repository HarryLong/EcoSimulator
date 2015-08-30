#include "environment_soil_humidity.h"
#include <math.h>
#include "../data_holders/pixel_data.h"

EnvironmentSoilHumidity::EnvironmentSoilHumidity(EnvironmentSpatialHashMap & map) : m_map(map)
{

}

int EnvironmentSoilHumidity::getSoilHumidity(QPoint p_center, float p_roots_size, int p_id)
{
    std::vector<EnvironmentSpatialHashMapCell*> cells(m_map.getCells(p_center, p_roots_size));

    int aggregated_humidity_percentage(0);

    for(auto it (cells.begin()) ; it != cells.end(); it++)
    {
        SoilHumidityCell* cell ((*it)->soil_humidity_cell);
        aggregated_humidity_percentage += cell->getGrantedHumidity(p_id);
    }
    return aggregated_humidity_percentage/cells.size(); // Return percentage
}

// DO NOT CALL THIS METHOD FOLLOWED BY GETHUMIDITY CONTRINUOUSLY RATHER UPDATE THIS FOR ALL NECESSARY CELLS IN ONE GO
void EnvironmentSoilHumidity::update(QPoint p_center, float p_roots_size, int p_id, int p_minimum_humidity)
{
    std::vector<EnvironmentSpatialHashMapCell*> cells(m_map.getCells(p_center, p_roots_size));

    for(auto it (cells.begin()) ; it != cells.end(); it++)
    {
        SoilHumidityCell* cell ((*it)->soil_humidity_cell);
        cell->update(p_id, p_roots_size, p_minimum_humidity);
    }
}

void EnvironmentSoilHumidity::remove(QPoint p_center, float p_roots_size, int p_id)
{
    std::vector<EnvironmentSpatialHashMapCell*> cells(m_map.getCells(p_center, p_roots_size));

    for(auto it (cells.begin()) ; it != cells.end(); it++)
    {
        SoilHumidityCell* cell ((*it)->soil_humidity_cell);
        cell->remove(p_id);
    }
}

//std::pair<int,int> EnvironmentSoilHumidity::getRange()
//{
//    return m_range;
//}
