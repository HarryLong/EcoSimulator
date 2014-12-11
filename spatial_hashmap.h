#ifndef SPATIAL_HASHMAP_H
#define SPATIAL_HASHMAP_H

#include <unordered_map>
#include <QPoint>
#include "helper.h"
#include "constants.h"
#include "math.h"

struct BoundingBox{
    int x_min;
    int x_max;
    int y_min;
    int y_max;
};

template <class T> class SpatialHashMap : protected unordered_map<QPoint, T>{
public:
    SpatialHashMap() :
        m_cell_width(SPATIAL_HASHMAP_CELL_WIDTH), m_cell_height(SPATIAL_HASHMAP_CELL_HEIGHT),
        m_horizontal_cell_count(ceil(((float)AREA_WIDTH_HEIGHT)/SPATIAL_HASHMAP_CELL_WIDTH)),
        m_vertical_cell_count(ceil(((float)AREA_WIDTH_HEIGHT)/SPATIAL_HASHMAP_CELL_HEIGHT)) {}

    int getHorizontalCellCount() const { return m_horizontal_cell_count; }
    int getVerticalCellCount() const { return m_vertical_cell_count; }
    int getCellWidth() const { return m_cell_width; }
    int getCellHeight() const { return m_cell_height; }

    void init_cell(QPoint key)
    {
        unordered_map<QPoint, T>::insert(std::pair<QPoint, T>(key, T()));
    }

    std::vector<T*> getCells(QPoint p_center, float p_radius)
    {
        // First calculate the bounding box
        float x_min(p_center.x()-p_radius);
        float x_max(p_center.x()+p_radius);

        float y_min(p_center.y()-p_radius);
        float y_max(p_center.y()+p_radius);

        BoundingBox bb;
        bb.x_min = max(0, (int) (x_min/m_cell_width));
        bb.x_max = min(m_horizontal_cell_count, (int) (x_max/m_cell_width) + 1);
        bb.y_min = max(0, (int) (y_min/m_cell_height));
        bb.y_max = min(m_vertical_cell_count,(int) (y_max/m_cell_height) +1);

        std::vector<T*> ret;
        for(int x (bb.x_min); x < bb.x_max; x++)
        {
            for(int y (bb.y_min); y < bb.y_max; y++)
            {
                // Ensure at least the center of the cell is within reach
                if(sqrt(pow(((x*m_cell_width) + m_cell_width/2) - p_center.x(),2) +
                      pow(((y*m_cell_height) + m_cell_height/2) - p_center.y(),2)) < p_radius)
                {
                    T* cell (get(QPoint(x,y)));
                    if(cell != NULL)
                        ret.push_back(cell);
                }
            }
        }

        // If no cells were added as the plant is too small, at least add the cell in which it is
        if(ret.empty())
            ret.push_back(get(QPoint(p_center.x()/m_cell_width, p_center.y()/m_cell_height)));

        return ret;
    }

    T* get(QPoint key)
    {
        auto it (unordered_map<QPoint, T>::find(key));
        if(it != unordered_map<QPoint, T>::end())
            return &(it->second);

        return NULL;
    }

    const T* get_const(QPoint key) const
    {
        auto it (unordered_map<QPoint, T>::find(key));
        if(it != unordered_map<QPoint, T>::end())
            return &(it->second);

        return NULL;
    }

    void clear()
    {
        unordered_map<QPoint, T>::clear();
    }

private:

    int m_cell_width, m_cell_height, m_horizontal_cell_count, m_vertical_cell_count;
};

#endif //SPATIAL_HASHMAP_H
