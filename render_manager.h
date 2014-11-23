#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <map>
#include <vector>
#include "renderer.h"
#include "QObject"

const static QString s_plant_renderer = "Plant Renderer";
const static QString s_lighting_renderer = "Lighting Renderer";
class RendererManager : public QObject
{
Q_OBJECT
public:
    RendererManager();

    void render(RenderData & render_data);

    std::vector<QString> getRendererNames();

    std::vector<Renderer*> getRendererWidgets();

public slots:
    void setActiveRenderer(QString p_renderer);

private:
    std::map<QString, Renderer*> m_renderers;
    QString m_active_renderer;

    void inactivate_all();
};

#endif //RENDER_MANAGER_H
