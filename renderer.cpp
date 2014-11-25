#include "renderer.h"

#include "constants.h"

#include <stdlib.h>
#include <initializer_list>
#include <iostream>
#include <QSize>
#include <QGridLayout>
#include "plant.h"
#include <random>
#include <QPainter>
#include "boost/foreach.hpp"

/*****************
 * BASE RENDERER *
 *****************/
Renderer::Renderer(QWidget *parent)
    : QWidget(parent), m_screen_space_multiplier(((float)RENDER_WINDOW_WIDTH_HEIGHT)/AREA_WIDTH_HEIGHT),
      m_render_ready(false)
{
}

Renderer::~Renderer()
{

}

QSize Renderer::minimumSizeHint() const
{
    return QSize(RENDER_WINDOW_WIDTH_HEIGHT, RENDER_WINDOW_WIDTH_HEIGHT);
}

QSize Renderer::sizeHint() const
{
    return QSize(RENDER_WINDOW_WIDTH_HEIGHT+100, RENDER_WINDOW_WIDTH_HEIGHT+100);
}

void Renderer::init_layout()
{
    setFixedSize(RENDER_WINDOW_WIDTH_HEIGHT, RENDER_WINDOW_WIDTH_HEIGHT);
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    setPalette(pal);
    setAutoFillBackground(true);
}

int Renderer::to_screen_space(float p_distance_in_cm)
{
    return (p_distance_in_cm * m_screen_space_multiplier);
}

/**********
 * PLANTS *
 **********/
PlantRenderer::PlantRenderer(QWidget *parent) :
    Renderer(parent)
{
    init_layout();
}

void PlantRenderer::render(RenderData p_render_data)
{
    m_plant_data = p_render_data.plant_render_data;
    m_render_ready = true;
    update();
}

void PlantRenderer::paintEvent(QPaintEvent * event)
{
    if(! m_render_ready)
        return;

    QPainter painter(this);
    for(auto it = m_plant_data.begin(); it != m_plant_data.end(); it++)
    {
        int r ( to_screen_space(it->canopy_radius) );
        QPoint center(to_screen_space(it->center_position.x()), to_screen_space(it->center_position.y()));
        painter.setPen( it->color );
        painter.setBrush( it->color );
        painter.drawEllipse( center, r, r );
    }
}

/************
 * LIGHTING *
 ************/
LightingRenderer::LightingRenderer(QWidget *parent) :
    Renderer(parent)
{
    init_layout();
}

void LightingRenderer::render(RenderData p_render_data)
{
    m_illumination_spatial_hashmap = p_render_data.illumination_render_data;
    m_render_ready = true;
    update();
}

void LightingRenderer::paintEvent(QPaintEvent * event)
{
    static const QColor light(Qt::yellow);

    if(! m_render_ready)
        return;

    QPainter painter(this);

    int cell_width(m_illumination_spatial_hashmap.getCellWidth());
    int cell_width_screen_space(to_screen_space(cell_width));
    int cell_height(m_illumination_spatial_hashmap.getCellHeight());
    int cell_height_screen_space(to_screen_space(cell_height));

    for(int x ( 0 ); x < m_illumination_spatial_hashmap.getHorizontalCellCount(); x++)
    {
        int x_screen_space(to_screen_space(cell_width * x));

        for(int y ( 0 ); y < m_illumination_spatial_hashmap.getVerticalCellCount(); y++)
        {
            int y_screen_space(to_screen_space(cell_height * y));

            IlluminationCellContent * cell_content(m_illumination_spatial_hashmap.get(QPoint(x,y)));
            if(cell_content->lit && cell_content->max_height == .0f)
                painter.fillRect(QRect(x_screen_space, y_screen_space, cell_width_screen_space, cell_height_screen_space), QBrush(light));
        }
    }
}

/*********
 * ROOTS *
 *********/
RootsRenderer::RootsRenderer(QWidget *parent) :
    Renderer(parent)
{
    init_layout();
}

void RootsRenderer::render(RenderData p_render_data)
{
    m_roots_data = p_render_data.plant_render_data;
    m_render_ready = true;
    update();
}

void RootsRenderer::paintEvent(QPaintEvent * event)
{
    if(! m_render_ready)
        return;

    QPainter painter(this);
    for(auto it = m_roots_data.begin(); it != m_roots_data.end(); it++)
    {
        int r ( to_screen_space(it->roots_radius) );
        QPoint center(to_screen_space(it->center_position.x()), to_screen_space(it->center_position.y()));
        painter.setPen( it->color );
        painter.setBrush( it->color );
        painter.drawEllipse( center, r, r );
    }
}

/*****************
 * SOIL HUMIDITY *
 *****************/
SoilHumidityRenderer::SoilHumidityRenderer(QWidget *parent) :
    Renderer(parent)
{
    init_layout();
}

void SoilHumidityRenderer::render(RenderData p_render_data)
{
    m_soil_humidity_spatial_hashmap = p_render_data.soil_humidity_render_data;
    m_render_ready = true;
    update();
}

void SoilHumidityRenderer::paintEvent(QPaintEvent * event)
{
    if(! m_render_ready)
        return;

    QPainter painter(this);

    int cell_width(m_soil_humidity_spatial_hashmap.getCellWidth());
    int cell_width_screen_space(to_screen_space(cell_width));
    int cell_height(m_soil_humidity_spatial_hashmap.getCellHeight());
    int cell_height_screen_space(to_screen_space(cell_height));

    for(int x ( 0 ); x < m_soil_humidity_spatial_hashmap.getHorizontalCellCount(); x++)
    {
        int x_screen_space(to_screen_space(cell_width * x));

        for(int y ( 0 ); y < m_soil_humidity_spatial_hashmap.getVerticalCellCount(); y++)
        {
            int y_screen_space(to_screen_space(cell_height * y));

            SoilHumidityCellContent * cell_content(m_soil_humidity_spatial_hashmap.get(QPoint(x,y)));
            if(cell_content->requests.size() == 0)
            {
                int humidity( cell_content->humidity_percentage );
                QColor color( 0, 0, (humidity / 100.0f) * 255);
                painter.fillRect(QRect(x_screen_space, y_screen_space, cell_width_screen_space, cell_height_screen_space),
                                 QBrush(color));
            }
        }
    }
}
