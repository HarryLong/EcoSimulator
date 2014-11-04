#include "window.h"

#include <QtWidgets>
#include <QApplication>
#include <QGridLayout>
#include "constants.h"

Window::Window() : m_simulator_manager()
{
    init_layout();
    m_simulator_manager.start();
}

Window::~Window()
{
}

void Window::init_layout()
{
    QSlider * time_control_slider = init_time_slider();

    QHBoxLayout * slider_layout = new QHBoxLayout();
    slider_layout->addWidget(new QLabel("Time: "),1);
    slider_layout->addWidget(time_control_slider,6);

    QGridLayout * main_layout = new QGridLayout;
    main_layout->addWidget(m_simulator_manager.getDisplay(),0,0,9,1);
    main_layout->addLayout(slider_layout,10,0,1,1, Qt::AlignBottom);

    setLayout(main_layout);
    setWindowTitle("ECOSYSTEM SIMULATOR");
}

QSlider * Window::init_time_slider()
{
    QSlider * time_control_slider = new QSlider(Qt::Horizontal);
    time_control_slider->setRange(TIME_SLIDER_MIN, TIME_SLIDER_MAX);
    time_control_slider->setSingleStep(1);
    time_control_slider->setTickInterval(1000);
    time_control_slider->setTickPosition(QSlider::TicksAbove);
    connect(time_control_slider, SIGNAL(valueChanged(int)), &m_simulator_manager, SLOT(setTimeAcceleration(int)));

    time_control_slider->setValue(TIME_SLIDER_DEFAULT);

    return time_control_slider;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window w;
    w.resize(w.sizeHint());
    w.showMaximized();

    return app.exec();
}
