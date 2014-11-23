#include "enviromnent_illumination.h"

EnvironmentIllumination::EnvironmentIllumination()
{

}

void EnvironmentIllumination::init()
{
    for(int x = 0; x < m_spatial_hashmap.getHorizontalCellCount(); x++)
    {
        for(int y = 0; y < m_spatial_hashmap.getVerticalCellCount(); y++)
        {
            m_spatial_hashmap.insert(Coordinate(x,y),IlluminationCellContent());
        }
    }
}

float EnvironmentIllumination::getMaxHeight(Coordinate p_cell_coord)
{
    m_spatial_hashmap.get(p_cell_coord)->max_height;
}

float EnvironmentIllumination::getIlluminationPercentage(Coordinate p_center, float p_radius, float p_height)
{
    BoundingBox bb(m_spatial_hashmap.getBoundingBox(p_center, p_radius));

    int illuminated_cell_count(0);
    int total_cell_count(0);
    for(int x = bb.x_min; x < bb.x_max; x++)
    {
        for(int y = bb.x_min; y < bb.x_max; y++)
        {
            IlluminationCellContent * cell_data (m_spatial_hashmap.get(Coordinate(x,y)));
            if(cell_data->lit && cell_data->max_height <= p_height)
                illuminated_cell_count++;

            total_cell_count++;
        }
    }
    return ((float)illuminated_cell_count)/total_cell_count;
}

void EnvironmentIllumination::setSize(Coordinate p_center, float p_radius, float height, int id)
{
    BoundingBox bb(m_spatial_hashmap.getBoundingBox(p_center, p_radius));

    for(int x = bb.x_min; x < bb.x_max; x++)
    {
        for(int y = bb.x_min; y < bb.x_max; y++)
        {
            IlluminationCellContent * cell_data (m_spatial_hashmap.get(Coordinate(x,y)));

            if(cell_data->lit) // Ignore if cell is in the shade
            {
                auto id_it(cell_data->id_to_height.find(id));
                if(id_it == cell_data->id_to_height.end()) // First time appearance of this id in the cell
                    cell_data->id_to_height.insert(std::pair<int,float>(id,height));
                else
                    id_it->second = height;

                // Check if an update to the maximum height of cell is necessary
                if(height > cell_data->max_height)
                    cell_data->max_height = height;
            }
        }
    }
}

void EnvironmentIllumination::remove(Coordinate p_center, float p_radius, int id)
{
    BoundingBox bb(m_spatial_hashmap.getBoundingBox(p_center, p_radius));

    for(int x = bb.x_min; x < bb.x_max; x++)
    {
        for(int y = bb.x_min; y < bb.x_max; y++)
        {
            IlluminationCellContent * cell_data (m_spatial_hashmap.get(Coordinate(x,y)));

            if(cell_data->lit) // Ignore if cell is in the shade
            {
                auto it (cell_data->id_to_height.find(id));
                float height(it->second);

                // Remove
                cell_data->id_to_height.erase(it);

                // If the plant was the maximum height, now find the next maximum
                if(height == cell_data->max_height ) // The id to remove is the maximum height so we need to find the next maximum
                {
                    if(cell_data->id_to_height.size() > 0) // There are ids left
                    {
                        float max_height(-1.f);

                        for(it = cell_data->id_to_height.begin(); it != cell_data->id_to_height.end(); it++)
                        {
                            if(it->second > max_height)
                                max_height = it->second;
                        }
                        cell_data->max_height = max_height;
                    }
                    else // There are no ids left
                        cell_data->max_height = .0f;
                }
            }
        }
    }
}

void EnvironmentIllumination::reset()
{
    for(int x = 0; x < m_spatial_hashmap.getHorizontalCellCount(); x++)
    {
        for(int y = 0; y < m_spatial_hashmap.getVerticalCellCount(); y++)
        {
            IlluminationCellContent * cell_data (m_spatial_hashmap.get(Coordinate(x,y)));
            cell_data->id_to_height.clear();
            cell_data->max_height = .0f;
        }
    }
}
