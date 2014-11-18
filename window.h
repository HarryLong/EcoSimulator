#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QScopedPointer>
#include <QComboBox>
#include <QSlider>
#include "display_area.h"
#include "time_manager.h"
#include "simulator_manager.h"
#include <map>


enum AltRenderType{
    NONE = 0,
    BOUNDING_BOX
};

struct AlternativeRenderers
{
    BoundingBoxRenderer * bounding_box;
};

class Window : public QWidget
{
    Q_OBJECT
public:
    Window();
    ~Window();

public slots:
    void updateRender();
    void setAltRenderer(int index);
    void setTimeAcceleration(int acceleration);

protected:
    virtual void closeEvent ( QCloseEvent * event );

private:
    void init_layout();
    void init_renderers();
    void init_signals();

    PlantRenderer * m_main_renderer;
    AlternativeRenderers m_alt_renderers;
    SimulatorManager m_simulator_manager;
    QSlider * m_time_control_slider;
    QLabel * m_trigger_frequency_lbl;
    QLabel * m_elapsed_time_lbl;
    QComboBox * m_alt_render_cb;
    std::map<int, Renderer*> m_alt_renderers_holder;

    int alt_render_selected_index;
};

#endif //WINDOW_H
