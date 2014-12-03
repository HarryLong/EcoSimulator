#include "enviromnent_illumination.h"
#include <QImage>

EnvironmentIllumination::EnvironmentIllumination() : data_set(false)
{
}

void EnvironmentIllumination::setIlluminationData(const QImage* p_image)
{
    QImage scaled_image (p_image->scaled(QSize(m_spatial_hashmap.getHorizontalCellCount(), m_spatial_hashmap.getVerticalCellCount()),
                                        Qt::IgnoreAspectRatio));

    for(int x = 0; x < m_spatial_hashmap.getHorizontalCellCount(); x++)
    {
        for(int y = 0; y < m_spatial_hashmap.getVerticalCellCount(); y++)
        {
            m_spatial_hashmap.insert(QPoint(x,y),IlluminationCellContent(qRed(scaled_image.pixel(x,y)) > 0));
        }
    }
    data_set = true;
}

float EnvironmentIllumination::getMaxHeight(QPoint p_cell_coord)
{
    return m_spatial_hashmap.get(p_cell_coord)->max_height;
}

int EnvironmentIllumination::getShadedPercentage(QPoint p_center, float p_canopy_radius, float p_height)
{
    BoundingBox bb(m_spatial_hashmap.getBoundingBox(p_center, p_canopy_radius));

    int shaded_cell_count(0);
    int total_cell_count(0);
    for(int x = bb.x_min; x < bb.x_max; x++)
    {
        for(int y = bb.y_min; y < bb.y_max; y++)
        {
            IlluminationCellContent * cell_data (m_spatial_hashmap.get(QPoint(x,y)));
            if(!cell_data->lit || cell_data->max_height > p_height)
                shaded_cell_count++;

            total_cell_count++;
        }
    }
    return (((float)shaded_cell_count)/total_cell_count) * 100; // Return percentage
}

void EnvironmentIllumination::setData(QPoint p_center, float p_canopy_radius, float p_height, int p_id)
{
    BoundingBox bb(m_spatial_hashmap.getBoundingBox(p_center, p_canopy_radius));

    for(int x = bb.x_min; x < bb.x_max; x++)
    {
        for(int y = bb.y_min; y < bb.y_max; y++)
        {
            IlluminationCellContent * cell_data (m_spatial_hashmap.get(QPoint(x,y)));

            if(cell_data->lit) // Ignore if cell is in the shade
            {
                auto id_it(cell_data->id_to_height.find(p_id));
                if(id_it == cell_data->id_to_height.end()) // First time appearance of this id in the cell
                    cell_data->id_to_height.insert(std::pair<int,float>(p_id,p_height));
                else
                    id_it->second = p_height;

                // Check if an update to the maximum height of cell is necessary
                if(p_height > cell_data->max_height)
                    cell_data->max_height = p_height;
            }
        }
    }
}

void EnvironmentIllumination::remove(QPoint p_center, float p_canopy_radius, int id)
{
    BoundingBox bb(m_spatial_hashmap.getBoundingBox(p_center, p_canopy_radius));

    for(int x = bb.x_min; x < bb.x_max; x++)
    {
        for(int y = bb.y_min; y < bb.y_max; y++)
        {
            IlluminationCellContent * cell_data (m_spatial_hashmap.get(QPoint(x,y)));

            if(cell_data->lit) // Ignore if cell is in the shade
            {
                float height(cell_data->id_to_height.find(id)->second);

                // If the plant was the maximum height, now find the next maximum
                if(height == cell_data->max_height ) // The id to remove is the maximum height so we need to find the next maximum
                {
                    float max_height(.0f);

                    for(auto it = cell_data->id_to_height.begin(); it != cell_data->id_to_height.end(); it++)
                    {
                        if(it->second != height && it->second > max_height)
                            max_height = it->second;
                    }
                    cell_data->max_height = max_height;
                }

                // Remove
                cell_data->id_to_height.erase(id);
            }
        }
    }
}

void EnvironmentIllumination::reset()
{
    m_spatial_hashmap.clear();
}
