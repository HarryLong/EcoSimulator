#ifndef SPATIAL_HASHMAP_H
#define SPATIAL_HASHMAP_H

#include <unordered_map>
#include "helper.h"

#include "constants.h"

#define CELL_WIDTH 50 // Centimeters
#define CELL_HEIGHT 50 // Centimeters

struct BoundingBox{
    int x_min;
    int x_max;
    int y_min;
    int y_max;
};

template <class T> class SpatialHashMap : protected unordered_map<Coordinate, T>{
public:
    SpatialHashMap() :
        m_cell_width(CELL_WIDTH), m_cell_height(CELL_HEIGHT),
        m_horizontal_cell_count(ceil(((float)AREA_WIDTH_HEIGHT)/CELL_WIDTH)),
        m_vertical_cell_count(ceil(((float)AREA_WIDTH_HEIGHT)/CELL_HEIGHT)) {}

    int getHorizontalCellCount() { return m_horizontal_cell_count; }
    int getVerticalCellCount() { return m_vertical_cell_count; }
    int getCellWidth() { return m_cell_width; }
    int getCellHeight() { return m_cell_height; }

    void insert(Coordinate key, T value)
    {
        unordered_map<Coordinate, T>::insert(std::pair<Coordinate, T>(key, value));
    }

    BoundingBox getBoundingBox(Coordinate p_center, float p_radius)
    {
        float x_min(p_center.x-p_radius);
        float x_max(p_center.x+p_radius);

        float y_min(p_center.y-p_radius);
        float y_max(p_center.y+p_radius);

        BoundingBox ret;

        ret.x_min = (x_min/m_cell_width);
        ret.x_max = (ceil(x_max/m_cell_width));
        ret.y_min = (y_min/m_cell_height);
        ret.y_max = (ceil(y_max/m_cell_height));

        return ret;
    }

    T* get(Coordinate key)
    {
        auto it (unordered_map<Coordinate, T>::find(key));
        if(it != unordered_map<Coordinate, T>::end())
            return &(it->second);

        return NULL;
    }

private:
    int m_cell_width, m_cell_height, m_horizontal_cell_count, m_vertical_cell_count;
};

#endif //SPATIAL_HASHMAP_H
