#ifndef RENDERER_H
#define RENDERER_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <qnamespace.h>
#include <QVector>
#include <QRgb>
#include "helper.h"

#include <unordered_set>
#include "environment_manager.h"

class Plant;

struct PlantRenderingData{
    std::string name;
    QColor color;
    Coordinate center_position;
    float height;
    float canopy_radius;

    PlantRenderingData(std::string p_name, QColor p_color, Coordinate p_center_position,
                       float p_height, float p_canopy_radius) : name(p_name), color(p_color),
        center_position(p_center_position), height(p_height), canopy_radius(p_canopy_radius) {}

    PlantRenderingData() : name("unset"), color(QColor(Qt::black)), center_position(Coordinate(0,0)), height(.0f), canopy_radius(.0f) {}
};

typedef std::vector<PlantRenderingData> PlantRenderDataContainer;
struct RenderData {
    PlantRenderDataContainer plant_render_data;
    IlluminationSpatialHashMap environmnent_render_data;
};

class Renderer : public QWidget
{
public:
    Renderer(QWidget *parent = 0);
    virtual void render(RenderData render_data) = 0;
    virtual ~Renderer();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

protected:
    virtual void init_layout() = 0;
    int to_screen_space(float p_distance_in_cm);

    float m_screen_space_multiplier;
};

class PlantRenderer : public Renderer
{
    Q_OBJECT
public:
    PlantRenderer(QWidget *parent = 0);
    virtual void render(RenderData p_render_data);

protected:
    virtual void paintEvent(QPaintEvent * event);
    virtual void init_layout();

private:
    PlantRenderDataContainer m_plant_data;
};

class LightingRenderer : public Renderer
{
    Q_OBJECT
public:
    LightingRenderer(QWidget *parent = 0);
    virtual void render(RenderData p_render_data);

protected:
    virtual void init_layout();

private:
    virtual void paintEvent(QPaintEvent * event);
    IlluminationSpatialHashMap m_illumination_spatial_hashmap;
};
#endif //RENDERER_H
