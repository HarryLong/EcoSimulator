#include "central_widget.h"

#include "overview_widget.h"

#include <QGridLayout>
#include <QSlider>
#include <QComboBox>
#include <functional>
#include <QMovie>
#include <QDebug>
#include <QCheckBox>
#include <QFile>

/************************
 * ANIMATED PUSH BUTTON *
 ************************/
AnimatedPushButton::AnimatedPushButton(const QString &text, const QString & link_to_animation, QWidget *parent) : QPushButton(text, parent),
    m_animation(new QMovie(link_to_animation)), m_text(text)
{
    connect(m_animation, SIGNAL(frameChanged(int)), this, SLOT(frame_changed(int)));
}

AnimatedPushButton::~AnimatedPushButton()
{
    delete m_animation;
}

void AnimatedPushButton::startAnimation()
{
    setEnabled(false);
    m_animation->start();
}

void AnimatedPushButton::stopAnimation()
{
    m_animation->stop();
    setIcon(QIcon());
    setEnabled(true);
}

void AnimatedPushButton::frame_changed(int frame)
{
    setIcon(QIcon(m_animation->currentPixmap()));

    if(frame == m_animation->frameCount()-1)
        startAnimation();
}

/******************
 * CENTRAL WIDGET *
 ******************/
#define MAX_UNIT_TIME 5000
#define TIME_SLIDER_MAX 4950 // 5000-4950 = 50
#define TIME_SLIDER_DEFAULT 2000 // 1000ms
#define TIME_SLIDER_MIN 0 // 5000 ms
CentralWidget::CentralWidget(QWidget * parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    m_simulator_manager(),
    m_start_config_dialog(this),
    m_render_manager(SimulatorManager::_AREA_WIDTH_HEIGHT, SimulatorManager::_AREA_WIDTH_HEIGHT,
                     std::bind(&SimulatorManager::getPlantRenderingData, &m_simulator_manager),
                     std::bind(&SimulatorManager::getEnvironmentRenderingData, &m_simulator_manager)),
    m_generate_statistical_snapshot_btn(new AnimatedPushButton("Generate Statistical Snapshot", ":/icons/loading.gif")),
    m_enable_render_cb(new QCheckBox(this))
{
    std::function<void()> callback_function(
                std::bind(&AnimatedPushButton::stopAnimation, m_generate_statistical_snapshot_btn));
    m_statistical_callback_listener = new CallbackListener(callback_function);

    init_widgets();
    init_layout();
    init_signals();

    m_enable_render_cb->setChecked(true);
    m_renderers_cb->setCurrentIndex(static_cast<int>(RendererManager::_DEFAULT_RENDER_TYPE));
    active_renderer(true);
}

CentralWidget::~CentralWidget()
{

}

void CentralWidget::active_renderer(bool activate)
{
    m_renderers_cb->setEnabled(activate);
    m_simulator_manager.generate_rendering_data(activate);
    if(activate)
    {
        m_render_manager.start();
        m_render_manager.show();
    }
    else
    {
        m_render_manager.stop();
        m_render_manager.hide();
    }
//    m_overview_widget->adjustSize();
//    this->adjustSize();
}

void CentralWidget::init_layout()
{
    // Time slider
    m_time_control_slider = new QSlider(Qt::Horizontal);
    m_time_control_slider->setRange(TIME_SLIDER_MIN, TIME_SLIDER_MAX);
    m_time_control_slider->setSingleStep(1);
    m_time_control_slider->setTickInterval(1000);
    m_time_control_slider->setTickPosition(QSlider::TicksAbove);

    QHBoxLayout * slider_layout = new QHBoxLayout();
    slider_layout->addWidget(new QLabel("Time: "),1);
    slider_layout->addWidget(m_time_control_slider,7);

    QVBoxLayout * info_heading_layout = new QVBoxLayout();
    {
        // Trigger freuqncy label
        {
            m_trigger_frequency_lbl = new QLabel();
            info_heading_layout->addWidget(m_trigger_frequency_lbl, 0, Qt::AlignCenter);
        }

        // Elapsed time label
        {
            m_elapsed_time_lbl = new QLabel();
            update_elapsed_time_label(0);
            info_heading_layout->addWidget(m_elapsed_time_lbl, 0, Qt::AlignCenter);
        }

        // QButtons
        {
            m_stop_start_button = new QPushButton(START_BTN_TEXT);
            m_pause_resume_button = new QPushButton(PAUSE_BTN_TEXT);
            m_pause_resume_button->setEnabled(false);
            info_heading_layout->addWidget(m_stop_start_button, 0, Qt::AlignCenter);
            info_heading_layout->addWidget(m_pause_resume_button, 0, Qt::AlignCenter);
        }

        {
            QHBoxLayout * enable_rendering_layout(new QHBoxLayout);

            // Renderers combo box
            m_renderers_cb = new QComboBox();
            QString * render_names(m_render_manager.getRendererNames());
            for(int i(0); i < RendererManager::RendererTypes::_N_RENDERERS; i++)
                m_renderers_cb->addItem(render_names[i]);


            enable_rendering_layout->addWidget(m_enable_render_cb, 0, Qt::AlignCenter);
            enable_rendering_layout->addWidget(m_renderers_cb,0, Qt::AlignCenter);
            info_heading_layout->addLayout(enable_rendering_layout, 0);
        }

    }

    QGridLayout * main_layout = new QGridLayout;
    main_layout->addLayout(info_heading_layout,0,0,1,2, Qt::AlignCenter);
    // Add renderers
    Renderer ** renderers(m_render_manager.getRendererWidgets());
    for(int i(0); i < RendererManager::RendererTypes::_N_RENDERERS; i++)
        main_layout->addWidget(renderers[i],1,0,9,2, Qt::AlignLeft);

    main_layout->addWidget(m_overview_widget,1,0,9,2, Qt::AlignRight);

    // Buttons
    {
        m_generate_snapshot_btn = new QPushButton("Generate Snapshot");

        QHBoxLayout * buttons_layout = new QHBoxLayout;
        buttons_layout->addWidget(m_generate_snapshot_btn);
        buttons_layout->addWidget(m_generate_statistical_snapshot_btn);

        main_layout->addLayout(buttons_layout,10,0,1,2, Qt::AlignCenter);
    }

    main_layout->addLayout(slider_layout,11,0,1,2, Qt::AlignBottom);

    setLayout(main_layout);
    setWindowTitle("ECOSYSTEM SIMULATOR");
}

void CentralWidget::init_widgets()
{
    m_overview_widget = new OverViewWidget();
}

void CentralWidget::init_signals()
{
    // Start button
    connect(m_stop_start_button, SIGNAL(clicked()), this, SLOT(start_stop_btn_clicked()));
    connect(m_pause_resume_button, SIGNAL(clicked()), this, SLOT(pause_resume_btn_clicked()));

    // Slider accelerates times
    connect(m_time_control_slider, SIGNAL(valueChanged(int)), this, SLOT(setTimeAcceleration(int)));

    m_time_control_slider->setValue(TIME_SLIDER_DEFAULT); // Triggers the default value

    // Simulator manager update triggers a re-render
    connect(&m_simulator_manager, SIGNAL(updated(int)), this, SLOT(updateRender()));

    connect(m_renderers_cb, SIGNAL( currentIndexChanged(int)) , &m_render_manager, SLOT( setActiveRenderer(int) ));

    connect(m_enable_render_cb, SIGNAL(clicked(bool)), this, SLOT(active_renderer(bool)));

    // The overview widget to the simulator
    connect(&m_simulator_manager, SIGNAL(newPlant(QString, QColor)), m_overview_widget, SLOT(addPlant(QString, QColor)));
    connect(&m_simulator_manager, SIGNAL(removedPlant(QString, QString)), m_overview_widget, SLOT(removePlant(QString,QString)));

    // The overview widget render filter
    connect(m_overview_widget, SIGNAL(filter(QString)), &m_render_manager, SLOT(filter(QString)));
    connect(m_overview_widget, SIGNAL(unfilter(QString)), &m_render_manager, SLOT(unfilter(QString)));

    // Snapshot generation
    connect(m_generate_snapshot_btn, SIGNAL(clicked()), &m_simulator_manager, SLOT(generateSnapshot()));
    connect(m_generate_statistical_snapshot_btn, SIGNAL(clicked()), this, SLOT(generate_statistical_snapshot()));

    // Config dialog
    connect(&m_start_config_dialog, SIGNAL(accepted()), this, SLOT(start_simulation()));
}

void CentralWidget::updateRender()
{
    // Update info
    update_elapsed_time_label(m_simulator_manager.getElapsedMonths());
}

void CentralWidget::update_elapsed_time_label(int p_months)
{
    int elapsed_years(p_months/12);
    int elapsed_months(p_months%12);
    m_elapsed_time_lbl->setText(QString("Elapsed time: %1 years %2 months (%3 months)").arg(elapsed_years)
                                .arg(elapsed_months).arg(p_months));
}

void CentralWidget::setTimeAcceleration(int p_acceleration)
{
    int unit_time (0);
    if(p_acceleration == TIME_SLIDER_MAX)
        m_trigger_frequency_lbl->setText(QString("Trigger frequency: MAX"));
    else
    {
        unit_time = MAX_UNIT_TIME+1-p_acceleration;
        m_trigger_frequency_lbl->setText(QString("Trigger frequency: %1 ms").arg(unit_time));
    }

    m_simulator_manager.setMonthlyTriggerFrequency(unit_time);
}

void CentralWidget::start_stop_btn_clicked()
{
    if(m_simulator_manager.getState() == SimulatorManager::State::Stopped) // Starting
        display_start_configuration_dialog();
    else // Stopping
        stop_simulation();
}

void CentralWidget::pause_resume_btn_clicked()
{
    if(m_simulator_manager.getState() == SimulatorManager::State::Paused) // Resuming
    {
        m_simulator_manager.resume();
        m_pause_resume_button->setText("Pause");
    }
    else // can only be RUNNING as the button is disabled if it is stopped
    {
        m_simulator_manager.pause();
        m_pause_resume_button->setText("Resume");
    }
}

void CentralWidget::start_simulation()
{
    m_stop_start_button->setText("Stop");
    // Enable pause/resume button
    m_pause_resume_button->setEnabled(true);
    m_simulator_manager.start(m_start_config_dialog.getStartConfiguration());
    m_render_manager.start();
}

void CentralWidget::stop_simulation()
{
    if(m_simulator_manager.getState() != SimulatorManager::State::Stopped ) // Starting
    {
        m_simulator_manager.stop();
        m_overview_widget->reset();
        m_stop_start_button->setText("Start");
        m_pause_resume_button->setText("Pause");

        m_render_manager.trigger(); // Make sure it triggers one last time to clear
        m_render_manager.stop();

        // Disable pause/resume button
        m_pause_resume_button->setEnabled(false);
    }
}

void CentralWidget::display_start_configuration_dialog()
{
    m_start_config_dialog.reset();
    m_start_config_dialog.exec();
}

void CentralWidget::generate_statistical_snapshot()
{
    m_generate_statistical_snapshot_btn->startAnimation();
    m_simulator_manager.generateStatisticalSnapshot(m_statistical_callback_listener);
}
