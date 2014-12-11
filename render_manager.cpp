#include "render_manager.h"


RendererManager::RendererManager(const PlantRenderDataContainer & plant_render_data, const EnvironmentSpatialHashMap & environment_render_data) :
    m_active_renderer(s_plant_renderer), m_time_manager()
{
    m_renderers.insert(std::pair<QString, Renderer*>(s_plant_renderer, new PlantRenderer(plant_render_data)));
    m_renderers.insert(std::pair<QString, Renderer*>(s_roots_renderer, new RootsRenderer(plant_render_data)));
    m_renderers.insert(std::pair<QString, Renderer*>(s_lighting_renderer, new IlluminationRenderer(environment_render_data)));
    m_renderers.insert(std::pair<QString, Renderer*>(s_soil_humidity_renderer, new SoilHumidityRenderer(environment_render_data)));

    setActiveRenderer(s_plant_renderer);

    m_time_manager.addListener(this);
    m_time_manager.setUnitTime(RENDER_INTERVAL);
    m_time_manager.start();
}

void RendererManager::trigger()
{
    m_renderers.find(m_active_renderer)->second->render();
}

void RendererManager::setActiveRenderer(QString p_renderer)
{
    m_active_renderer = p_renderer;
    inactivate_all();
    m_renderers.find(m_active_renderer)->second->setVisible(true);
}

std::vector<QString> RendererManager::getRendererNames()
{
    std::vector<QString > ret;
    for(auto it (m_renderers.begin()); it != m_renderers.end(); it++)
        ret.push_back(it->first);
    return ret;
}

std::vector<Renderer*> RendererManager::getRendererWidgets()
{
    std::vector<Renderer*> ret;
    for(auto it (m_renderers.begin()); it != m_renderers.end(); it++)
        ret.push_back(it->second);
    return ret;
}

void RendererManager::inactivate_all()
{
    for(auto it (m_renderers.begin()); it != m_renderers.end(); it++)
    {
        it->second->setVisible(false);
    }
}

void RendererManager::filter(QString p_plant_name)
{
    m_renderers.find(s_plant_renderer)->second->filter(p_plant_name);
    m_renderers.find(s_roots_renderer)->second->filter(p_plant_name);
}

void RendererManager::unfilter(QString p_plant_name)
{
    m_renderers.find(s_plant_renderer)->second->unfilter(p_plant_name);
    m_renderers.find(s_roots_renderer)->second->unfilter(p_plant_name);
}
