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
Renderer::Renderer(int p_width, int p_height, QWidget *parent)
    : QWidget(parent), m_width(p_width), m_height(p_height)
{
}

Renderer::~Renderer()
{

}

QSize Renderer::minimumSizeHint() const
{
    return QSize(m_width, m_height);
}

QSize Renderer::sizeHint() const
{
    return QSize(m_width+100, m_height+100);
}

/******************
 * PLANT RENDERER *
 ******************/
PlantRenderer::PlantRenderer(int p_width, int p_height, QWidget *parent) :
    Renderer(p_width, p_height, parent)
{
    init_layout();
}

void PlantRenderer::init_layout()
{
    setFixedSize(m_width, m_height);
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    setPalette(pal);
    setAutoFillBackground(true);
}

void PlantRenderer::render(PlantRenderDataContainer p_render_data)
{
    m_plant_data = p_render_data;
    update();
}

void PlantRenderer::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    for(auto it = m_plant_data.begin(); it != m_plant_data.end(); it++)
    {
        int r ( (it->canopy_radius/100.f) * METER_IN_PIXELS );
        QPoint center(it->center_position.x, it->center_position.y);
        painter.setPen( it->color );
        painter.setBrush( it->color );
        painter.drawEllipse( center, r, r );
    }
}

/****************
 * BOUNDING BOX *
 ****************/
BoundingBoxRenderer::BoundingBoxRenderer(int p_width, int p_height, QWidget *parent) :
    Renderer(p_width, p_height, parent), m_container_label()
{
    QImage init_image(m_width, m_height, QImage::Format::Format_RGB32);
    init_image.fill(Qt::black);
    set_image(init_image);

    init_layout();
}

void BoundingBoxRenderer::init_layout()
{
    QGridLayout * main_layout = new QGridLayout;
    main_layout->addWidget(&m_container_label,0,0,1,1, Qt::AlignCenter);
    setLayout(main_layout);
}

void BoundingBoxRenderer::render(PlantBoundingBoxContainer p_render_data)
{
    static QRgb white(QColor(Qt::white).rgb());

    QImage image(m_width, m_height, QImage::Format::Format_RGB32);
    image.fill(Qt::black);

    for(int x = 0; x < m_width; x++)
    {
        for(int y = 0; y < m_width; y++)
        {
            int index((y*m_width) + x);

            if(!p_render_data[index]->empty())
                image.setPixel(x, y, white);
        }
    }
    set_image(image);
}

void BoundingBoxRenderer::set_image(QImage & image)
{
    m_container_label.setPixmap(QPixmap::fromImage(image));
}

