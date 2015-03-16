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
#include <iostream>
#include <mutex>
#include <math.h>

/*****************
 * BASE RENDERER *
 *****************/
Renderer::Renderer(QWidget *parent)
    : QWidget(parent), m_screen_space_multiplier(((float)RENDER_WINDOW_WIDTH_HEIGHT)/AREA_WIDTH_HEIGHT),
      m_filters()
{
}

Renderer::~Renderer()
{

}

void Renderer::render()
{
    update();
}

QSize Renderer::minimumSizeHint() const
{
    return QSize(RENDER_WINDOW_WIDTH_HEIGHT, RENDER_WINDOW_WIDTH_HEIGHT);
}

QSize Renderer::sizeHint() const
{
    return QSize(RENDER_WINDOW_WIDTH_HEIGHT+100, RENDER_WINDOW_WIDTH_HEIGHT+100);
}

void Renderer::filter(QString p_plant_name)
{
    m_filters.insert(p_plant_name);
}

void Renderer::unfilter(QString p_plant_name)
{
    m_filters.erase(p_plant_name);
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
    return std::round(p_distance_in_cm * m_screen_space_multiplier);
}

/**********
 * PLANTS *
 **********/
PlantRenderer::PlantRenderer(const PlantRenderDataContainer & render_data, QWidget *parent) :
    Renderer(parent),
    m_render_data(render_data)
{
    init_layout();
}

void PlantRenderer::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    m_render_data.lock();
    for(auto it = m_render_data.begin(); it != m_render_data.end(); it++)
    {
        if(m_filters.find(it->name) == m_filters.end())
        {
            int diameter ( to_screen_space(2*it->canopy_radius) );
            QPoint center(to_screen_space(it->center_position.x()), to_screen_space(it->center_position.y()));
            painter.setPen( *(it->color) );
            painter.setBrush( *(it->color) );
            int r(std::max(1,(int)std::round(diameter/2)));
            painter.drawEllipse( center, r, r);
        }
    }
    m_render_data.unlock();
}

/*********
 * ROOTS *
 *********/
RootsRenderer::RootsRenderer(const PlantRenderDataContainer & render_data, QWidget *parent) :
    Renderer(parent),
    m_render_data(render_data)
{
    init_layout();
}

void RootsRenderer::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    m_render_data.lock();
    for(auto it = m_render_data.begin(); it != m_render_data.end(); it++)
    {
        if(m_filters.find(it->name) == m_filters.end())
        {
            int r ( std::max(1,to_screen_space(it->roots_radius)) );
            QPoint center(to_screen_space(it->center_position.x()), to_screen_space(it->center_position.y()));
            painter.setPen( *(it->color) );
            painter.setBrush( *(it->color) );
            painter.drawEllipse( center, r, r );
        }
    }
    m_render_data.unlock();
}

/**************************
 * BASE RESOURCE RENDERER *
 **************************/
ResourceRenderer::ResourceRenderer(const EnvironmentSpatialHashMap & render_data, const PixelDataTranslator * translator, QWidget *parent) :
    Renderer(parent),
    m_render_data(render_data),
    m_translator(translator)
{

}

ResourceRenderer::~ResourceRenderer()
{
    delete m_translator;
}

void ResourceRenderer::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);

    int cell_width(m_render_data.getCellWidth());
    int cell_width_screen_space(to_screen_space(cell_width)+1);
    int cell_height(m_render_data.getCellHeight());
    int cell_height_screen_space(to_screen_space(cell_height)+1);

    for(int x ( 0 ); x < m_render_data.getHorizontalCellCount(); x++)
    {
        int x_screen_space(to_screen_space(cell_width * x));

        for(int y ( 0 ); y < m_render_data.getVerticalCellCount(); y++)
        {
            int y_screen_space(to_screen_space(cell_height * y));
            QColor color(getPixel(QPoint(x,y)));
            painter.fillRect(QRect(x_screen_space, y_screen_space, cell_width_screen_space, cell_height_screen_space), QBrush(color));
        }
    }
}

/************
 * LIGHTING *
 ************/
IlluminationRenderer::IlluminationRenderer(const EnvironmentSpatialHashMap & render_data, QWidget *parent) :
    ResourceRenderer(render_data, new IlluminationTranslator, parent)
{
    init_layout();
}

QRgb IlluminationRenderer::getPixel(QPoint pos)
{
    IlluminationCell * cell_content(m_render_data.get_const(pos)->illumination_cell);
    return m_translator->toRGB(cell_content->get());
}

/*****************
 * SOIL HUMIDITY *
 *****************/
SoilHumidityRenderer::SoilHumidityRenderer(const EnvironmentSpatialHashMap & render_data, QWidget *parent) :
    ResourceRenderer(render_data, new SoilHumidityTranslator, parent)
{
    init_layout();
}

QRgb SoilHumidityRenderer::getPixel(QPoint pos)
{
    SoilHumidityCell * cell_content(m_render_data.get_const(pos)->soil_humidity_cell);
    return m_translator->toRGB(cell_content->get());
}

/************************
 * TEMPERATURE RENDERER *
 ***********************/
TemperatureRenderer::TemperatureRenderer(const EnvironmentSpatialHashMap & render_data, QWidget *parent) :
    ResourceRenderer(render_data, new TemperatureTranslator, parent)
{
    init_layout();
}

QRgb TemperatureRenderer::getPixel(QPoint pos)
{
    TemperatureCell * cell_content(m_render_data.get_const(pos)->temp_cell);
    return m_translator->toRGB(cell_content->get());
}
