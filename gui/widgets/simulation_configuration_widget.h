#ifndef SIMULATION_CONFIGURATION_WIDGET_H
#define SIMULATION_CONFIGURATION_WIDGET_H

#include <QWidget>

#include "../../simulator/core/simulation_configuration.h"

class QPushButton;
class QTableWidget;
class QCheckBox;
class QButtonGroup;

/***********************************
 * SIMULATION CONFIGURATION WIDGET *
 ***********************************/
class SimulationConfigurationWidget : public QWidget
{
    Q_OBJECT
public:
    SimulationConfigurationWidget( QWidget * parent = 0 );
    ~SimulationConfigurationWidget();
    bool seedingEnabled();
//    SimulationOptions getSimulationConfiguration();

private:
    void init_layout();

    QButtonGroup * m_seeding_button_group;
    QCheckBox * m_seeding_enabled_cb;
    QCheckBox * m_seeding_disabled_cb;

    int m_width, m_height;
};

#endif // SIMULATION_CONFIGURATION_WIDGET_H
