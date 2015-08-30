#include "environment_illumination.h"
#include <QImage>
#include <math.h>

#include "../data_holders/pixel_data.h"

#include <QDebug>

EnvironmentIllumination::EnvironmentIllumination(EnvironmentSpatialHashMap & map) : m_map(map)
{

}

//float EnvironmentIllumination::getMaxHeight(QPoint p_cell_coord)
//{
//    return m_map.get(p_cell_coord)->illumination_cell->m_max_height;
//}

#define HEIGHT_BUFFER 5 //cm
int EnvironmentIllumination::getDailyIllumination(QPoint p_center, int p_id, float p_canopy_width, float height)
{
    std::vector<EnvironmentSpatialHashMapCell*> cells(m_map.getCells(p_center, p_canopy_width/2));

    int aggregated_daily_illumination(0);

    for(auto it (cells.begin()) ; it != cells.end(); it++)
    {
        IlluminationCell* cell((*it)->illumination_cell);

        aggregated_daily_illumination += cell->getIllumination(p_id, height);
    }
    return aggregated_daily_illumination/cells.size(); // Divide by cells iterated over
}

void EnvironmentIllumination::update(QPoint p_center, float p_canopy_width, float p_height, int p_id)
{
    std::vector<EnvironmentSpatialHashMapCell*> cells(m_map.getCells(p_center, p_canopy_width/2));

    for(auto it (cells.begin()) ; it != cells.end(); it++)
    {
        IlluminationCell * cell((*it)->illumination_cell);
        cell->update(p_id, p_height);

//        auto id_it(cell->id_to_height.find(p_id));
//        if(id_it == cell->id_to_height.end()) // First time appearance of this id in the cell
//            cell->id_to_height.insert(std::pair<int,float>(p_id,p_height));
//        else
//            id_it->second = p_height;

//        // Check if an update to the maximum height of cell is necessary
//        if(p_height > cell->m_max_height)
//            cell->m_max_height = p_height;
    }
}

void EnvironmentIllumination::remove(QPoint p_center, float p_canopy_width, int p_id)
{
    std::vector<EnvironmentSpatialHashMapCell*> cells(m_map.getCells(p_center, p_canopy_width/2));

    for(auto it (cells.begin()) ; it != cells.end(); it++)
    {
        IlluminationCell* cell((*it)->illumination_cell);
        cell->remove(p_id);
//        float height(cell->id_to_height.find(id)->second);
//        cell->id_to_height.erase(id);

//        float max_height(.0f);

//        // If the plant was the maximum height, now find the next maximum
//        if(height == cell->m_max_height ) // The id to remove is the maximum height so we need to find the next maximum
//        {
//            for(auto it2 ( cell->id_to_height.begin()); it2 != cell->id_to_height.end(); it2++)
//            {
//                if(it2->second > max_height)
//                    max_height = it2->second;
//            }
//            cell->m_max_height = max_height;
//        }
    }
}
