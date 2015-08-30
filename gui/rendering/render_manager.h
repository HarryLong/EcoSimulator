#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <map>
#include <vector>
#include "renderer.h"
#include "QObject"
#include <functional>
#include "../../utils/time_manager.h"

class RendererManager : public QObject, public TimeManager::TimeListener
{
Q_OBJECT
public:
    enum RendererTypes{
        _PLANT = 0,
        _ROOTS,
        _LIGHTING,
        _SOIL_HUMIDITY,
        _TEMPERATURE,
        _N_RENDERERS
    };

    static const RendererTypes _DEFAULT_RENDER_TYPE = RendererTypes::_PLANT;

    RendererManager(int area_width, int area_height,
                    std::function<const PlantRenderDataContainer&()> plant_rendering_data_retriever_fn,
                    std::function<const EnvironmentSpatialHashMap&()> environmental_rendering_data_retriever_fn);

    QString * getRendererNames();
    Renderer** getRendererWidgets();

    virtual void trigger();
    void stop();
    void start();

public slots:
    void setActiveRenderer(int index);
    void setActiveRenderer(RendererTypes p_render_type);
    void filter(QString p_plant_name);
    void unfilter(QString p_plant_name);

private:
    QString m_render_names[_N_RENDERERS];
    Renderer * m_renderers[_N_RENDERERS];
    int m_current_month;
    RendererTypes m_active_renderer;
    TimeManager m_time_manager;

    void inactivate_all();
};

#endif //RENDER_MANAGER_H
