#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QScopedPointer>
#include <QComboBox>
#include <QSlider>
#include "time_manager.h"
#include "simulator_manager.h"
#include <map>
#include "overview_widget.h"
#include "render_manager.h"
#include <QPushButton>

const static QString START_BTN_TEXT = "Start";
const static QString STOP_BTN_TEXT = "Stop";
const static QString RESUME_BTN_TEXT = "Resume";
const static QString PAUSE_BTN_TEXT = "Pause";

class Window : public QWidget
{
    Q_OBJECT
public:
    Window();
    ~Window();

public slots:
    void updateRender();
    void setTimeAcceleration(int acceleration);

protected:
    virtual void closeEvent ( QCloseEvent * event );

private slots:
    void start_stop_btn_clicked();
    void pause_resume_btn_clicked();

private:
    void init_layout();
    void init_widgets();
    void init_signals();

    RendererManager m_render_manager;

    SimulatorManager m_simulator_manager;
    OverViewWidget * m_overview_widget;
    std::map<int, Renderer*> m_renderers_holder;

    // UI ELEMENTS
    QSlider * m_time_control_slider;
    QLabel * m_trigger_frequency_lbl;
    QLabel * m_elapsed_time_lbl;
    QComboBox * m_renderers_cb;
    QPushButton * m_stop_start_button;
    QPushButton * m_pause_resume_button;
};

#endif //WINDOW_H
