#include "central_widget.h"

#include <QGridLayout>
#include "constants.h"

#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include"plant_db_editor.h"

CentralWidget::CentralWidget(QWidget * parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    m_simulator_manager(), m_start_config_dialog(this)
{
    init_widgets();
    init_layout();
    init_signals();
}

CentralWidget::~CentralWidget()
{
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
        // Renderers combo box
        QHBoxLayout * heading_layout = new QHBoxLayout();
        m_renderers_cb = new QComboBox();

        for(QString renderer_name : m_render_manager.getRendererNames())
            m_renderers_cb->addItem(renderer_name);

        heading_layout->addWidget(new QLabel("Active Renderer: "));
        heading_layout->addWidget(m_renderers_cb);

        // Trigger freuqncy label
        m_trigger_frequency_lbl = new QLabel();

        // Elapsed time label
        m_elapsed_time_lbl = new QLabel();
        update_elapsed_time_label(0);

        // QButtons
        m_stop_start_button = new QPushButton(START_BTN_TEXT);
        m_pause_resume_button = new QPushButton(PAUSE_BTN_TEXT);
        m_pause_resume_button->setEnabled(false);

        info_heading_layout->addLayout(heading_layout);
        info_heading_layout->addWidget(m_trigger_frequency_lbl);
        info_heading_layout->addWidget(m_elapsed_time_lbl);
        info_heading_layout->addWidget(m_stop_start_button);
        info_heading_layout->addWidget(m_pause_resume_button);
    }

    QGridLayout * main_layout = new QGridLayout;
    main_layout->addLayout(info_heading_layout,0,0,1,2, Qt::AlignCenter);

    // Add renderers
    for(QWidget* renderer : m_render_manager.getRendererWidgets())
        main_layout->addWidget(renderer,1,0,9,1, Qt::AlignCenter);

    main_layout->addWidget(m_overview_widget,1,1,9,1, Qt::AlignCenter);
    main_layout->addLayout(slider_layout,10,0,1,2, Qt::AlignBottom);

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
    connect(&m_simulator_manager, SIGNAL(update()),
            this, SLOT(updateRender()));

    connect(m_renderers_cb, SIGNAL( currentIndexChanged(QString) ), &m_render_manager, SLOT( setActiveRenderer(QString) ));
    m_renderers_cb->setCurrentText(m_render_manager.getDefaultRederName());

    // The overview widget to the simulator
    connect(&m_simulator_manager, SIGNAL(newPlant(QString, QColor)), m_overview_widget, SLOT(addPlant(QString,QColor)));
    connect(&m_simulator_manager, SIGNAL(removedPlant(QString, QString)), m_overview_widget, SLOT(removePlant(QString,QString)));

    // The start config dialog upon accepted should forward data to the simulator
    connect(&m_start_config_dialog, SIGNAL(accepted()), this, SLOT(start_simulation()));

    // The overview widget render filter
    connect(m_overview_widget, SIGNAL(filter(QString)), &m_render_manager, SLOT(filter(QString)));
    connect(m_overview_widget, SIGNAL(unfilter(QString)), &m_render_manager, SLOT(unfilter(QString)));
}

void CentralWidget::updateRender()
{
    RenderData render_data(
                m_simulator_manager.getPlantRenderingData(),
                m_simulator_manager.getIlluminationRenderingData(),
                m_simulator_manager.getSoilHumidityRenderingData()
                );

    m_render_manager.render(render_data);

    // Update info
    update_elapsed_time_label(m_simulator_manager.getElapsedMonths());
}

void CentralWidget::update_elapsed_time_label(int p_months)
{
    int elapsed_years(p_months/12);
    int elapsed_months(p_months%12);
    m_elapsed_time_lbl->setText(QString("Elapsed time: %1 years %2 months").arg(elapsed_years).arg(elapsed_months));
}

void CentralWidget::setTimeAcceleration(int p_acceleration)
{
    int unit_time(MAX_UNIT_TIME+1-p_acceleration);
    m_trigger_frequency_lbl->setText(QString("Trigger frequency: %1 ms").arg(unit_time));
    m_simulator_manager.setMonthlyTriggerFrequency(unit_time);
}

void CentralWidget::start_stop_btn_clicked()
{
    if(m_simulator_manager.getState() == Stopped) // Starting
        m_start_config_dialog.exec();
    else // Stopping
    {
        m_simulator_manager.stop();
        m_overview_widget->reset();
        m_stop_start_button->setText("Start");
        m_pause_resume_button->setText("Pause");

        // Disable pause/resume button
        m_pause_resume_button->setEnabled(false);
    }
}

void CentralWidget::pause_resume_btn_clicked()
{
    if(m_simulator_manager.getState() == Paused) // Resuming
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
    StartConfiguration start_config(m_start_config_dialog.getStartConfiguration());

    // Add the plants to create
    m_simulator_manager.setIllumination(start_config.illumination);
    m_simulator_manager.setSoilHumidity(start_config.soil_humidity);
    m_simulator_manager.addPlants(start_config.m_plants);

    m_stop_start_button->setText("Stop");
    // Enable pause/resume button
    m_pause_resume_button->setEnabled(true);
    m_simulator_manager.start();
}

void CentralWidget::closeEvent ( QCloseEvent * event )
{
    std::cout << "Stopping..." << std::endl;
    m_simulator_manager.stop();
}

void CentralWidget::display_start_configuration_dialog()
{
    m_start_config_dialog.exec();
}
