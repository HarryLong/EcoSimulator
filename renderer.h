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
typedef std::vector<std::unordered_set<int>*> PlantBoundingBoxContainer;

class Renderer : public QWidget
{
public:
    Renderer(int p_width, int p_height, QWidget *parent = 0);
    virtual ~Renderer();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

protected:
    virtual void init_layout() = 0;

    int m_width, m_height;
};

class PlantRenderer : public Renderer
{
    Q_OBJECT
public:
    PlantRenderer(int p_width, int p_height, QWidget *parent = 0);
    virtual void render(PlantRenderDataContainer render_data);

protected:
    virtual void paintEvent(QPaintEvent * event);
    virtual void init_layout();

private:
    PlantRenderDataContainer m_plant_data;
};

#define COLORS Qt::red, Qt::yellow, Qt::green, Qt::white, Qt::blue, Qt::magenta
#define COLOR_COUNT 6
class BoundingBoxRenderer : public Renderer
{
    Q_OBJECT
public:
    BoundingBoxRenderer(int p_width, int p_height, QWidget *parent = 0);
    virtual void render(PlantBoundingBoxContainer render_data);

protected:
    virtual void init_layout();

private:
    void set_image(QImage & image);
    QLabel m_container_label;

    // Test method
    QRgb get_random_color();
};
#endif //RENDERER_H
