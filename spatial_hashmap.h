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

    int getHorizontalCellCount() { return m_horizontal_cell_count; }
    int getVerticalCellCount() { return m_vertical_cell_count; }
    int getCellWidth() { return m_cell_width; }
    int getCellHeight() { return m_cell_height; }

    void insert(QPoint key, T value)
    {
        unordered_map<QPoint, T>::insert(std::pair<QPoint, T>(key, value));
    }

    BoundingBox getBoundingBox(QPoint p_center, float p_radius)
    {
        float x_min(p_center.x()-p_radius);
        float x_max(p_center.x()+p_radius);

        float y_min(p_center.y()-p_radius);
        float y_max(p_center.y()+p_radius);

        BoundingBox ret;

        ret.x_min = max(0, (int) (x_min/m_cell_width));
        ret.x_max = min(m_horizontal_cell_count, (int) (x_max/m_cell_width) + 1);
        ret.y_min = max(0, (int) (y_min/m_cell_height));
        ret.y_max = min(m_vertical_cell_count,(int) (y_max/m_cell_height) +1);

        return ret;
    }

    T* get(QPoint key)
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
