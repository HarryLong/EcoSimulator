#include "environment_temp.h"

EnvironmentTemperature::EnvironmentTemperature(EnvironmentSpatialHashMap & map) : m_map(map)
{
}

void EnvironmentTemperature::setTemperatureData(PixelData * p_data, int variance)
{
    if(p_data->m_width != m_map.getHorizontalCellCount() ||
            p_data->m_height != m_map.getVerticalCellCount())
    {
        p_data->scale(m_map.getHorizontalCellCount(), m_map.getVerticalCellCount());
    }

    for(int x = 0; x < m_map.getHorizontalCellCount(); x++)
    {
        for(int y = 0; y < m_map.getVerticalCellCount(); y++)
        {
            int min(p_data->getValue(QPoint(x,y)));
            m_map.insertTempCell(QPoint(x,y),new TemperatureCell(Range(min, min+variance)));
        }
    }
}

int EnvironmentTemperature::getTemperature(int p_month, QPoint p_center)
{
    return m_map.getCells(p_center, 0)[0]->temp_cell->get(p_month);
}

