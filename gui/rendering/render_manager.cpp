#include "render_manager.h"
#include <QDebug>

#define RENDER_INTERVAL 200 //ms

RendererManager::RendererManager(int area_width, int area_height,
                                 std::function<const PlantRenderDataContainer&()> plant_rendering_data_retriever_fn,
                                 std::function<const EnvironmentSpatialHashMap&()> environmental_rendering_data_retriever_fn) :
    m_time_manager(), m_current_month(1)
{
    m_renderers[RendererTypes::_PLANT] = new PlantRenderer(area_width, area_height, plant_rendering_data_retriever_fn);
    m_renderers[RendererTypes::_ROOTS] = new RootsRenderer(area_width, area_height, plant_rendering_data_retriever_fn);
    m_renderers[RendererTypes::_LIGHTING] = new IlluminationRenderer(area_width, area_height, environmental_rendering_data_retriever_fn);
    m_renderers[RendererTypes::_SOIL_HUMIDITY] = new SoilHumidityRenderer(area_width, area_height, environmental_rendering_data_retriever_fn);
    m_renderers[RendererTypes::_TEMPERATURE] = new TemperatureRenderer(area_width, area_height, environmental_rendering_data_retriever_fn);

    m_render_names[RendererTypes::_PLANT] = "Plant Renderer";
    m_render_names[RendererTypes::_ROOTS] = "Roots Renderer";
    m_render_names[RendererTypes::_LIGHTING] = "Lighting Renderer";
    m_render_names[RendererTypes::_SOIL_HUMIDITY] = "Soil humidity renderer";
    m_render_names[RendererTypes::_TEMPERATURE] = "Temperature renderer";

    setActiveRenderer(RendererTypes::_PLANT);

    m_time_manager.addListener(this);
    m_time_manager.setUnitTime(RENDER_INTERVAL);
}

void RendererManager::stop()
{
    m_time_manager.stop();
}

void RendererManager::start()
{
    m_time_manager.start();
}

void RendererManager::trigger()
{
    m_renderers[m_active_renderer]->render();
}

void RendererManager::setActiveRenderer(RendererTypes p_render_type)
{
    m_active_renderer = p_render_type;
    inactivate_all();
    m_renderers[m_active_renderer]->setVisible(true);
}

void RendererManager::setActiveRenderer(int index)
{
    setActiveRenderer(static_cast<RendererTypes>(index));
}

QString * RendererManager::getRendererNames()
{
    return m_render_names;
}

Renderer** RendererManager::getRendererWidgets()
{
    return m_renderers;
}

void RendererManager::inactivate_all()
{
    for(int i(0); i < RendererTypes::_N_RENDERERS; i++)
        m_renderers[i]->setVisible(false);
}

void RendererManager::filter(QString p_plant_name)
{
    static_cast<PlantRenderer*>(m_renderers[RendererTypes::_PLANT])->filter(p_plant_name);
    static_cast<PlantRenderer*>(m_renderers[RendererTypes::_ROOTS])->filter(p_plant_name);
}

void RendererManager::unfilter(QString p_plant_name)
{
    static_cast<PlantRenderer*>(m_renderers[RendererTypes::_PLANT])->unfilter(p_plant_name);
    static_cast<PlantRenderer*>(m_renderers[RendererTypes::_ROOTS])->unfilter(p_plant_name);
}
