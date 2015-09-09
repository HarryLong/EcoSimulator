#ifndef PLANT_RENDERING_DATA_H
#define PLANT_RENDERING_DATA_H

#include <QString>
#include <QPoint>
#include <QColor>

class PlantRenderingData{
public:
    QString name;
    QPoint center_position;
    const QColor color;
    float height;
    float roots_radius;
    float canopy_radius;

    PlantRenderingData(QString p_name, const QColor p_color, QPoint p_center_position, float p_height, float p_canopy_width,
                       float p_roots_size) : name(p_name), color(p_color), center_position(p_center_position), height(p_height),
        canopy_radius(p_canopy_width/2), roots_radius(p_roots_size) {}
};

#endif // PLANT_RENDERING_DATA_H
