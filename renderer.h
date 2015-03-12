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
#include <mutex>
#include "pixel_data_translator.h"

class Plant;

struct PlantRenderingData{
    QString name;
    QPoint center_position;
    const QColor* color;
    float height;
    float roots_radius;
    float canopy_radius;

    PlantRenderingData(QString p_name, const QColor* p_color, QPoint p_center_position, float p_height, float p_canopy_width,
                       float p_roots_size) : name(p_name), color(p_color), center_position(p_center_position), height(p_height),
        canopy_radius(p_canopy_width/2), roots_radius(p_roots_size) {}

};

struct PlantRenderDataContainer : public std::vector<PlantRenderingData>
{
public:
    PlantRenderDataContainer() : mutex() {}

    void lock() const
    {
        mutex.lock();
    }

    void unlock() const
    {
        mutex.unlock();
    }

private:
    mutable std::mutex mutex;
};

/*****************
 * BASE RENDERER *
 *****************/
class Renderer : public QWidget
{
public:
    Renderer(QWidget *parent);
    virtual void render();
    virtual ~Renderer();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void filter(QString p_plant_name);
    void unfilter(QString p_plant_name);

protected:
    virtual void init_layout();
    int to_screen_space(float p_distance_in_cm);

    float m_screen_space_multiplier;
    std::set<QString> m_filters;
};

/**********
 * PLANTS *
 **********/
class PlantRenderer : public Renderer
{
    Q_OBJECT
public:
    PlantRenderer(const PlantRenderDataContainer & render_data, QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent * event);

private:
    const PlantRenderDataContainer & m_render_data;
};

/*********
 * ROOTS *
 *********/
class RootsRenderer : public Renderer
{
    Q_OBJECT
public:
    RootsRenderer(const PlantRenderDataContainer & render_data, QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent * event);

private:
    const PlantRenderDataContainer & m_render_data;
};

/**************************
 * BASE RESOURCE RENDERER *
 **************************/
class ResourceRenderer : public Renderer
{
public:
    ResourceRenderer(const EnvironmentSpatialHashMap & render_data, const PixelDataTranslator * translator, QWidget *parent = 0);
    virtual ~ResourceRenderer();

    virtual void paintEvent(QPaintEvent * event);

    virtual QRgb getPixel(QPoint pos) = 0;
protected:
    const PixelDataTranslator * m_translator;
    const EnvironmentSpatialHashMap & m_render_data;
};

/************
 * LIGHTING *
 ************/
class IlluminationRenderer : public ResourceRenderer
{
    Q_OBJECT
public:
    IlluminationRenderer(const EnvironmentSpatialHashMap & render_data, QWidget *parent = 0);
    QRgb getPixel(QPoint pos);
};

/*****************
 * SOIL HUMIDITY *
 *****************/
class SoilHumidityRenderer : public ResourceRenderer
{
    Q_OBJECT
public:
    SoilHumidityRenderer(const EnvironmentSpatialHashMap & render_data, QWidget *parent = 0);
    QRgb getPixel(QPoint pos);
};

/************************
 * TEMPERATURE RENDERER *
 ***********************/
class TemperatureRenderer : public ResourceRenderer
{
    Q_OBJECT
public:
    TemperatureRenderer(const EnvironmentSpatialHashMap & render_data, QWidget *parent = 0);
    QRgb getPixel(QPoint pos);
};
#endif //RENDERER_H
