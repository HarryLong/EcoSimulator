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
    QPoint center_position;
    float height;
    float roots_radius;
    float canopy_radius;

    PlantRenderingData(std::string p_name, QColor p_color, QPoint p_center_position,
                       float p_height, float p_canopy_radius, float p_roots_radius) : name(p_name), color(p_color),
        center_position(p_center_position), height(p_height), canopy_radius(p_canopy_radius), roots_radius(p_roots_radius) {}

//    PlantRenderingData() : name("unset"), color(QColor(Qt::black)), center_position(QPoint(0,0)), height(.0f), canopy_radius(.0f), roots_radius(.0f) {}
};

typedef std::vector<PlantRenderingData> PlantRenderDataContainer;
struct RenderData {
    PlantRenderDataContainer plant_render_data;
    IlluminationSpatialHashMap illumination_render_data;
    SoilHumiditySpatialHashMap soil_humidity_render_data;

    RenderData(PlantRenderDataContainer p_plant_render_data, IlluminationSpatialHashMap p_illumination_render_data, SoilHumiditySpatialHashMap p_soil_humidity_render_data) :
        plant_render_data(p_plant_render_data),
        illumination_render_data(p_illumination_render_data),
        soil_humidity_render_data(p_soil_humidity_render_data) {}
};

/*****************
 * BASE RENDERER *
 *****************/
class Renderer : public QWidget
{
public:
    Renderer(QWidget *parent = 0);
    virtual void render(RenderData render_data) = 0;
    virtual ~Renderer();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

protected:
    virtual void init_layout();
    int to_screen_space(float p_distance_in_cm);

    float m_screen_space_multiplier;
    bool m_render_ready;
};

/**********
 * PLANTS *
 **********/
class PlantRenderer : public Renderer
{
    Q_OBJECT
public:
    PlantRenderer(QWidget *parent = 0);
    virtual void render(RenderData p_render_data);

protected:
    virtual void paintEvent(QPaintEvent * event);

private:
    PlantRenderDataContainer m_plant_data;
};

/*********
 * ROOTS *
 *********/
class RootsRenderer : public Renderer
{
    Q_OBJECT
public:
    RootsRenderer(QWidget *parent = 0);
    virtual void render(RenderData p_render_data);

protected:
    virtual void paintEvent(QPaintEvent * event);

private:
    PlantRenderDataContainer m_roots_data;
};

/************
 * LIGHTING *
 ************/
class LightingRenderer : public Renderer
{
    Q_OBJECT
public:
    LightingRenderer(QWidget *parent = 0);
    virtual void render(RenderData p_render_data);

protected:

private:
    virtual void paintEvent(QPaintEvent * event);
    IlluminationSpatialHashMap m_illumination_spatial_hashmap;
};

/*****************
 * SOIL HUMIDITY *
 *****************/
class SoilHumidityRenderer : public Renderer
{
    Q_OBJECT
public:
    SoilHumidityRenderer(QWidget *parent = 0);
    virtual void render(RenderData p_render_data);

protected:

private:
    virtual void paintEvent(QPaintEvent * event);
    SoilHumiditySpatialHashMap m_soil_humidity_spatial_hashmap;
};
#endif //RENDERER_H
