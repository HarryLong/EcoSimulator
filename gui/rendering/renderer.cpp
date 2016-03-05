#include "renderer.h"

#include <stdlib.h>
#include <initializer_list>
#include <iostream>
#include <QSize>
#include <QGridLayout>
#include <random>
#include <QPainter>
#include "boost/foreach.hpp"
#include <iostream>
#include <mutex>
#include <math.h>

#define RENDER_WINDOW_WIDTH_HEIGHT 700

/*****************
 * BASE RENDERER *
 *****************/
Renderer::Renderer(int area_width, int area_height, QWidget *parent)
    : QWidget(parent), m_screen_space_multiplier(((float)RENDER_WINDOW_WIDTH_HEIGHT)/area_width)
{
    init_layout();
}

Renderer::~Renderer()
{

}

void Renderer::render()
{
    update();
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
PlantRenderer::PlantRenderer(int area_width, int area_height,
                             std::function<const PlantRenderDataContainer&()> plant_rendering_data_retriever_fn, QWidget * parent ) :
    Renderer(area_width, area_height, parent),
    m_plant_rendering_data_retriever_fn(plant_rendering_data_retriever_fn)
{

}

PlantRenderer::~PlantRenderer()
{

}

void PlantRenderer::filter(QString p_plant_name)
{
    m_filters.insert(p_plant_name);
}

void PlantRenderer::unfilter(QString p_plant_name)
{
    m_filters.erase(p_plant_name);
}

void PlantRenderer::paintEvent(QPaintEvent * event)
{
    const PlantRenderDataContainer & plant_render_data(m_plant_rendering_data_retriever_fn());

    plant_render_data.lock();

    QPainter painter(this);
    for(auto it = plant_render_data.begin(); it != plant_render_data.end(); it++)
    {
        if(m_filters.find(it->name) == m_filters.end())
        {
            int diameter ( to_screen_space(2*it->canopy_radius) );
            QPoint center(to_screen_space(it->center_position.x()), to_screen_space(it->center_position.y()));
            painter.setPen( it->color );
            painter.setBrush( it->color );
            int r(std::max(1,(int)std::round(diameter/2)));
            painter.drawEllipse( center, r, r);
        }
    }

    plant_render_data.unlock();
}

/*********
 * ROOTS *
 *********/
RootsRenderer::RootsRenderer(int area_width, int area_height, std::function<const PlantRenderDataContainer&()> plant_rendering_data_retriever_fn, QWidget *parent) :
    PlantRenderer(area_width, area_height, plant_rendering_data_retriever_fn, parent)
{

}

void RootsRenderer::paintEvent(QPaintEvent * event)
{
    const PlantRenderDataContainer & plant_render_data(m_plant_rendering_data_retriever_fn());

    plant_render_data.lock();

    QPainter painter(this);
    for(auto it = plant_render_data.begin(); it != plant_render_data.end(); it++)
    {
        if(m_filters.find(it->name) == m_filters.end())
        {
            int r ( std::max(1,to_screen_space(it->roots_radius)) );
            QPoint center(to_screen_space(it->center_position.x()), to_screen_space(it->center_position.y()));
            painter.setPen( it->color );
            painter.setBrush( it->color );
            painter.drawEllipse( center, r, r );
        }
    }

    plant_render_data.unlock();
}

/**************************
 * BASE RESOURCE RENDERER *
 **************************/
ResourceRenderer::ResourceRenderer(int area_width, int area_height, std::function<const EnvironmentSpatialHashMap&()> environmental_rendering_data_retriever_fn,
                                   ResourceConverter * resource_visual_converter, QWidget *parent) :
    Renderer(area_width, area_height, parent),
    m_environmental_rendering_data_retriever_fn(environmental_rendering_data_retriever_fn),
    m_resource_visual_converter(resource_visual_converter)
{

}

ResourceRenderer::~ResourceRenderer()
{
    delete m_resource_visual_converter;
}

void ResourceRenderer::paintEvent(QPaintEvent * event)
{
    const EnvironmentSpatialHashMap& environment_resources(m_environmental_rendering_data_retriever_fn());

    QPainter painter(this);

    int cell_width(environment_resources.getCellWidth());
    int cell_width_screen_space(to_screen_space(cell_width)+1);
    int cell_height(environment_resources.getCellHeight());
    int cell_height_screen_space(to_screen_space(cell_height)+1);

    for(int x ( 0 ); x < environment_resources.getHorizontalCellCount(); x++)
    {
        int x_screen_space(to_screen_space(cell_width * x));

        for(int y ( 0 ); y < environment_resources.getVerticalCellCount(); y++)
        {
            QPoint cell(x,y);
            int y_screen_space(to_screen_space(cell_height * y));
            QColor color(Qt::black);
            if(environment_resources.initialised(cell))
            {
                int resource_value(getResource(environment_resources, QPoint(x,y)));
                color = QColor(m_resource_visual_converter->toRGB(resource_value));
            }
            painter.fillRect(QRect(x_screen_space, y_screen_space, cell_width_screen_space, cell_height_screen_space), QBrush(color));
        }
    }
}

/************
 * LIGHTING *
 ************/
IlluminationRenderer::IlluminationRenderer(int area_width, int area_height, std::function<const EnvironmentSpatialHashMap&()> environmental_rendering_data_retriever_fn,
                                           QWidget *parent) :
    ResourceRenderer(area_width, area_height, environmental_rendering_data_retriever_fn, new IlluminationConverter, parent)
{
}

int IlluminationRenderer::getResource(const EnvironmentSpatialHashMap & environment_spatial_hashmap, QPoint pos)
{
    return environment_spatial_hashmap.getCell(pos, EnvironmentSpatialHashMap::Space::_HASHMAP).illumination_cell.getRenderingIllumination();
}

/*****************
 * SOIL HUMIDITY *
 *****************/
SoilHumidityRenderer::SoilHumidityRenderer(int area_width, int area_height, std::function<const EnvironmentSpatialHashMap&()> environmental_rendering_data_retriever_fn,
                                           QWidget *parent) :
    ResourceRenderer(area_width, area_height, environmental_rendering_data_retriever_fn, new SoilHumidityConverter, parent)
{
}

int SoilHumidityRenderer::getResource(const EnvironmentSpatialHashMap & environment_spatial_hashmap, QPoint pos)
{
    return environment_spatial_hashmap.getCell(pos, EnvironmentSpatialHashMap::Space::_HASHMAP).soil_humidity_cell.getRenderingHumidity();
}

/************************
 * TEMPERATURE RENDERER *
 ***********************/
TemperatureRenderer::TemperatureRenderer(int area_width, int area_height, std::function<const EnvironmentSpatialHashMap&()> environmental_rendering_data_retriever_fn,
                                         QWidget *parent) :
    ResourceRenderer(area_width, area_height, environmental_rendering_data_retriever_fn, new TemperatureConverter, parent)
{
}

int TemperatureRenderer::getResource(const EnvironmentSpatialHashMap & environment_spatial_hashmap, QPoint pos)
{
    return environment_spatial_hashmap.getCell(pos, EnvironmentSpatialHashMap::Space::_HASHMAP).temp_cell.getTemperature();
//    return QRgb();
//    TemperatureCell * cell_content(m_render_data.get_const(pos)->temp_cell);
//    return m_translator->toRGB(cell_content->get());
}
