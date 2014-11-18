#include "window.h"

#include <QtWidgets>
#include <QApplication>
#include <QGridLayout>
#include "constants.h"

Window::Window() : m_simulator_manager(), m_alt_renderers_holder(), alt_render_selected_index(AltRenderType::NONE)
{
    init_renderers();
    init_layout();
    init_signals();
    m_simulator_manager.start();
}

Window::~Window()
{
}

void Window::init_layout()
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
        // Alternative renderers combo box
        QHBoxLayout * heading_layout = new QHBoxLayout();
        m_alt_render_cb = new QComboBox();
        m_alt_render_cb->addItem("None");
        m_alt_render_cb->addItem("Bounding Box");
        heading_layout->addWidget(new QLabel("Alternative Layout: "));
        heading_layout->addWidget(m_alt_render_cb);

        // Trigger freuqncy label
        m_trigger_frequency_lbl = new QLabel();

        // Elapsed time label
        m_elapsed_time_lbl = new QLabel();

        info_heading_layout->addLayout(heading_layout);
        info_heading_layout->addWidget(m_trigger_frequency_lbl);
        info_heading_layout->addWidget(m_elapsed_time_lbl);
    }

    QGridLayout * main_layout = new QGridLayout;
    main_layout->addLayout(info_heading_layout,0,0,1,2, Qt::AlignCenter);
    main_layout->addWidget(m_main_renderer,1,0,9,1, Qt::AlignRight);
    main_layout->addWidget(m_alt_renderers.bounding_box,1,1,9,1, Qt::AlignLeft);
    main_layout->addLayout(slider_layout,10,0,1,2, Qt::AlignBottom);

    setLayout(main_layout);
    setWindowTitle("ECOSYSTEM SIMULATOR");
}

void Window::init_renderers()
{
    m_main_renderer = new PlantRenderer(AREA_WIDTH, AREA_HEIGHT);

    // Bounding box
    m_alt_renderers.bounding_box = new BoundingBoxRenderer(AREA_WIDTH, AREA_HEIGHT);
    m_alt_renderers.bounding_box->setVisible(false);
    m_alt_renderers_holder[AltRenderType::BOUNDING_BOX] = m_alt_renderers.bounding_box;
}

void Window::init_signals()
{
    // Slider accelerates times
    connect(m_time_control_slider, SIGNAL(valueChanged(int)), this, SLOT(setTimeAcceleration(int)));
    m_time_control_slider->setValue(TIME_SLIDER_DEFAULT); // Triggers the default value

    // Simulator manager update triggers a re-render
    connect(&m_simulator_manager, SIGNAL(update()),
            this, SLOT(updateRender()));

    connect(m_alt_render_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(setAltRenderer(int)));
}

void Window::updateRender()
{
    m_alt_renderers.bounding_box->render(m_simulator_manager.getBoundingBoxData());
    m_main_renderer->render(m_simulator_manager.getPlantData());

    int total_elapsed_months(m_simulator_manager.getElapsedMonths());
    int elapsed_years(total_elapsed_months/12);
    int elapsed_months(total_elapsed_months%12);
    m_elapsed_time_lbl->setText(QString("Elapsed time: %1 years %2 months").arg(elapsed_years).arg(elapsed_months));
}

void Window::setAltRenderer(int index)
{
    if(alt_render_selected_index != AltRenderType::NONE)
        m_alt_renderers_holder[alt_render_selected_index]->setVisible(false);

    if(index != AltRenderType::NONE)
        m_alt_renderers_holder[index]->setVisible(true);

    alt_render_selected_index = index;
}

void Window::setTimeAcceleration(int p_acceleration)
{
    int unit_time(MAX_UNIT_TIME+1-p_acceleration);
    m_trigger_frequency_lbl->setText(QString("Trigger frequency: %1 ms").arg(unit_time));
    m_simulator_manager.setMonthlyTriggerFrequency(unit_time);
}

void Window::closeEvent ( QCloseEvent * event )
{
    std::cout << "Stopping..." << std::endl;
    m_simulator_manager.stop();
}

int main(int argc, char *argv[])
{
    std::cout << "Starting..." << std::endl;
    QApplication app(argc, argv);
    Window w;
    w.resize(w.sizeHint());
    w.showMaximized();

    return app.exec();
}
