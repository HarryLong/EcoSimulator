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

/************
 * RENDERER *
 ************/
Renderer::Renderer(QWidget *parent)
    : QWidget(parent), m_screen_space_multiplier(((float)RENDER_WINDOW_WIDTH_HEIGHT)/AREA_WIDTH_HEIGHT)
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

int Renderer::to_screen_space(float p_distance_in_cm)
{
    return (p_distance_in_cm * m_screen_space_multiplier);
}

/******************
 * PLANT RENDERER *
 ******************/
PlantRenderer::PlantRenderer(QWidget *parent) :
    Renderer(parent)
{
    init_layout();
}

void PlantRenderer::init_layout()
{
    setFixedSize(RENDER_WINDOW_WIDTH_HEIGHT, RENDER_WINDOW_WIDTH_HEIGHT);
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    setPalette(pal);
    setAutoFillBackground(true);
}

void PlantRenderer::render(RenderData p_render_data)
{
    m_plant_data = p_render_data.plant_render_data;
    update();
}

void PlantRenderer::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    for(auto it = m_plant_data.begin(); it != m_plant_data.end(); it++)
    {
        int r ( to_screen_space(it->canopy_radius) );
        QPoint center(to_screen_space(it->center_position.x), to_screen_space(it->center_position.y));
        painter.setPen( it->color );
        painter.setBrush( it->color );
        painter.drawEllipse( center, r, r );
    }
}

/****************
 * BOUNDING BOX *
 ****************/
LightingRenderer::LightingRenderer(QWidget *parent) :
    Renderer(parent)
{
    init_layout();
}

void LightingRenderer::init_layout()
{
    setFixedSize(RENDER_WINDOW_WIDTH_HEIGHT, RENDER_WINDOW_WIDTH_HEIGHT);
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    setPalette(pal);
    setAutoFillBackground(true);
}

void LightingRenderer::render(RenderData p_render_data)
{
    m_illumination_spatial_hashmap = p_render_data.environmnent_render_data;
    update();
}

void LightingRenderer::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    painter.setPen(QColor(Qt::white));

    int cell_width(m_illumination_spatial_hashmap.getCellWidth());
    int cell_width_screen_space(to_screen_space(cell_width));
    int cell_height(m_illumination_spatial_hashmap.getCellHeight());
    int cell_height_screen_space(to_screen_space(cell_height));

    int text_box_start_x_offset(cell_width_screen_space/4);
    int text_box_start_y_offset(cell_height_screen_space/4);
    int text_box_length(cell_height_screen_space/2);
    int text_box_width(cell_height_screen_space/2);

    bool draw_y_line(true);

    for(int x ( 0 ); x < m_illumination_spatial_hashmap.getHorizontalCellCount(); x++)
    {
        int x_screen_space(to_screen_space(cell_width * x));
        painter.drawLine(QPoint(x_screen_space, 0), QPoint(x_screen_space, RENDER_WINDOW_WIDTH_HEIGHT)); // Vertical lines

        for(int y ( 0 ); y < m_illumination_spatial_hashmap.getVerticalCellCount(); y++)
        {
            int y_screen_space(to_screen_space(cell_height * y));
            if(draw_y_line)
                painter.drawLine(QPoint(0, y_screen_space), QPoint(RENDER_WINDOW_WIDTH_HEIGHT, y_screen_space)); // Horizontal lines

            int max_height(m_illumination_spatial_hashmap.get(Coordinate(x,y))->max_height);

            // TODO: Set color (lit / not lit)
            painter.drawText(x_screen_space+text_box_start_x_offset, y_screen_space+text_box_start_y_offset,
                             text_box_length, text_box_width, Qt::AlignCenter, QString::number(max_height) );

        }
    }
}

