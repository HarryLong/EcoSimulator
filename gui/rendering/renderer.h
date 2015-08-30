#ifndef RENDERER_H
#define RENDERER_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <qnamespace.h>
#include <QVector>
#include <QRgb>
#include <unordered_set>

#include <functional>
#include "../../data_holders/environment_spatial_hashmap.h"
#include "../../data_holders/plant_rendering_data_container.h"
#include "resource_visual_converters.h"

class Plant;
/*****************
 * BASE RENDERER *
 *****************/
class Renderer : public QWidget
{
    Q_OBJECT
public:
    Renderer(int area_width, int area_height, QWidget *parent = 0);
    virtual ~Renderer();

    virtual void render();

protected:
    virtual void init_layout();
    int to_screen_space(float p_distance_in_cm);

    float m_screen_space_multiplier;
};

/**********
 * PLANTS *
 **********/
class PlantRenderer : public Renderer
{
    Q_OBJECT
public:
    PlantRenderer(int area_width, int area_height, std::function<const PlantRenderDataContainer&()> plant_rendering_data_retriever_fn, QWidget * parent = 0);
    virtual ~PlantRenderer();
    void filter(QString p_plant_name);
    void unfilter(QString p_plant_name);

protected:
    virtual void paintEvent(QPaintEvent * event);
    std::function<const PlantRenderDataContainer&()> m_plant_rendering_data_retriever_fn;
    std::set<QString> m_filters;

private:
};

/*********
 * ROOTS *
 *********/
class RootsRenderer : public PlantRenderer
{
    Q_OBJECT
public:
    RootsRenderer(int area_width, int area_height, std::function<const PlantRenderDataContainer&()> plant_rendering_data_retriever_fn, QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent * event);
};

/**************************
 * BASE RESOURCE RENDERER *
 **************************/
class ResourceRenderer : public Renderer
{
public:
    ResourceRenderer(int area_width, int area_height, std::function<const EnvironmentSpatialHashMap&()> environmental_rendering_data_retriever_fn,
                     ResourceConverter * resource_visual_converter, QWidget *parent = 0);
    virtual ~ResourceRenderer();

    virtual void paintEvent(QPaintEvent * event);

    virtual int getResource(const EnvironmentSpatialHashMap & environment_spatial_hashmap, QPoint pos) = 0;
protected:
    std::function<const EnvironmentSpatialHashMap&()> m_environmental_rendering_data_retriever_fn;

private:
    ResourceConverter * m_resource_visual_converter;
};

/************
 * LIGHTING *
 ************/
class IlluminationRenderer : public ResourceRenderer
{
    Q_OBJECT
public:
    IlluminationRenderer(int area_width, int area_height, std::function<const EnvironmentSpatialHashMap&()> environmental_rendering_data_retriever_fn,
                         QWidget *parent = 0);
    int getResource(const EnvironmentSpatialHashMap & environment_spatial_hashmap, QPoint pos);
};

/*****************
 * SOIL HUMIDITY *
 *****************/
class SoilHumidityRenderer : public ResourceRenderer
{
    Q_OBJECT
public:
    SoilHumidityRenderer(int area_width, int area_height, std::function<const EnvironmentSpatialHashMap&()> environmental_rendering_data_retriever_fn,
                         QWidget *parent = 0);
    int getResource(const EnvironmentSpatialHashMap & environment_spatial_hashmap, QPoint pos);
};

/************************
 * TEMPERATURE RENDERER *
 ***********************/
class TemperatureRenderer : public ResourceRenderer
{
    Q_OBJECT
public:
    TemperatureRenderer(int area_width, int area_height, std::function<const EnvironmentSpatialHashMap&()> environmental_rendering_data_retriever_fn,
                        QWidget *parent = 0);
    int getResource(const EnvironmentSpatialHashMap & environment_spatial_hashmap, QPoint pos);
};
#endif //RENDERER_H
