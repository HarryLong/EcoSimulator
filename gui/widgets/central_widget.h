#ifndef CENTRAL_WIDGET_H
#define CENTRAL_WIDGET_H

#include <QWidget>
#include <QScopedPointer>
#include <QComboBox>
#include <map>
#include <QMainWindow>
#include <QPushButton>

#include "../../simulator/core/simulator_manager.h"
#include "../rendering/render_manager.h"
#include "../dialogs/start_config_dialog.h"
#include "../../utils/callback_listener.h"

class QSlider;
class QComboBox;
class StartConfigDialog;
class OverViewWidget;
class QMovie;
class QCheckBox;

const static QString START_BTN_TEXT = "Start";
const static QString STOP_BTN_TEXT = "Stop";
const static QString RESUME_BTN_TEXT = "Resume";
const static QString PAUSE_BTN_TEXT = "Pause";

/************************
 * ANIMATED PUSH BUTTON *
 ************************/
class AnimatedPushButton : public QPushButton
{
Q_OBJECT
public:
    AnimatedPushButton(const QString &text, const QString & link_to_animation, QWidget *parent=0);
    ~AnimatedPushButton();

public slots:
    void startAnimation();
    void stopAnimation();

private slots:
    void frame_changed(int frame);

private:
    QMovie * m_animation;
    QString m_text;
};

/******************
 * CENTRAL WIDGET *
 ******************/
class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    CentralWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~CentralWidget();

public slots:
    void updateRender();
    void setTimeAcceleration(int acceleration);
    void stop_simulation();

private slots:
    void start_stop_btn_clicked();
    void pause_resume_btn_clicked();
    void display_start_configuration_dialog();
    void start_simulation();
    void generate_statistical_snapshot();
    void active_renderer(bool);

private:
    void init_layout();
    void init_widgets();
    void init_signals();
    void update_elapsed_time_label(int p_months);

    SimulatorManager m_simulator_manager;
    RendererManager m_render_manager;

    OverViewWidget * m_overview_widget;

    // DIALOGS
    StartConfigDialog m_start_config_dialog;

    // UI ELEMENTS
    QCheckBox * m_enable_render_cb;
    QSlider * m_time_control_slider;
    QPushButton * m_generate_snapshot_btn;
    AnimatedPushButton * m_generate_statistical_snapshot_btn;
    QLabel * m_trigger_frequency_lbl;
    QLabel * m_elapsed_time_lbl;
    QComboBox * m_renderers_cb;
    QPushButton * m_stop_start_button;
    QPushButton * m_pause_resume_button;
    CallbackListener * m_statistical_callback_listener;
};

#endif //CENTRAL_WIDGET_H
