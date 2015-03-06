#include "enviromnent_illumination.h"
#include <QImage>
#include <math.h>

EnvironmentIllumination::EnvironmentIllumination(EnvironmentSpatialHashMap & map) : m_map(map)
{
}

void EnvironmentIllumination::setIlluminationData(const QImage & p_image)
{
    QImage scaled_image (p_image.scaled(QSize(m_map.getHorizontalCellCount(), m_map.getVerticalCellCount()),
                                        Qt::IgnoreAspectRatio));

    for(int x = 0; x < m_map.getHorizontalCellCount(); x++)
    {
        for(int y = 0; y < m_map.getVerticalCellCount(); y++)
        {
            int daily_illumination(std::round((qRed(scaled_image.pixel(x,y)) / 255.f) * 24.0f));
            m_map.insertIlluminationCell(QPoint(x,y),new IlluminationCell(daily_illumination));
        }
    }
}

float EnvironmentIllumination::getMaxHeight(QPoint p_cell_coord)
{
    return m_map.get(p_cell_coord)->illumination_cell->max_height;
}

#define HEIGHT_BUFFER 5 //cm
int EnvironmentIllumination::getDailyIllumination(QPoint p_center, float p_canopy_width, float p_height)
{
    std::vector<EnvironmentSpatialHashMapCell*> cells(m_map.getCells(p_center, p_canopy_width/2));

    int aggregated_daily_illumination(0);

    for(auto it (cells.begin()) ; it != cells.end(); it++)
    {
        IlluminationCell* cell((*it)->illumination_cell);

        if(cell->max_height-HEIGHT_BUFFER <= p_height)
            aggregated_daily_illumination += cell->daily_illumination;
    }

    return aggregated_daily_illumination/cells.size(); // Return percentage
}

void EnvironmentIllumination::setData(QPoint p_center, float p_canopy_width, float p_height, int p_id)
{
    std::vector<EnvironmentSpatialHashMapCell*> cells(m_map.getCells(p_center, p_canopy_width/2));

    for(auto it (cells.begin()) ; it != cells.end(); it++)
    {
        IlluminationCell * cell((*it)->illumination_cell);

        auto id_it(cell->id_to_height.find(p_id));
        if(id_it == cell->id_to_height.end()) // First time appearance of this id in the cell
            cell->id_to_height.insert(std::pair<int,float>(p_id,p_height));
        else
            id_it->second = p_height;

        // Check if an update to the maximum height of cell is necessary
        if(p_height > cell->max_height)
            cell->max_height = p_height;
    }
}

void EnvironmentIllumination::remove(QPoint p_center, float p_canopy_width, int id)
{
    std::vector<EnvironmentSpatialHashMapCell*> cells(m_map.getCells(p_center, p_canopy_width/2));

    for(auto it (cells.begin()) ; it != cells.end(); it++)
    {
        IlluminationCell* cell((*it)->illumination_cell);
        float height(cell->id_to_height.find(id)->second);
        cell->id_to_height.erase(id);

        float max_height(.0f);

        // If the plant was the maximum height, now find the next maximum
        if(height == cell->max_height ) // The id to remove is the maximum height so we need to find the next maximum
        {
            for(auto it2 ( cell->id_to_height.begin()); it2 != cell->id_to_height.end(); it2++)
            {
                if(it2->second > max_height)
                    max_height = it2->second;
            }
            cell->max_height = max_height;
        }
    }
}
