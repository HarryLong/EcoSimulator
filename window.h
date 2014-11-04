#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QScopedPointer>
#include <QComboBox>
#include <QSlider>
#include "display_area.h"
#include "time_manager.h"
#include "simulator_manager.h"

class Window : public QWidget
{
    Q_OBJECT
public:
    Window();
    ~Window();

private:
    void init_layout();
    QSlider * init_time_slider();

    SimulatorManager m_simulator_manager;
};

#endif //WINDOW_H
