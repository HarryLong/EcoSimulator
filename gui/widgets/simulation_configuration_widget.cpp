#include "simulation_configuration_widget.h"
#include <QStringListModel>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <algorithm>
#include <QBoxLayout>
#include <QLabel>
#include <QButtonGroup>
#include <QSpinBox>

#define SIMULATION_CONFIGURATION_WIDGET_WIDTH_HEIGHT 700

/***********************************
 * SIMULATION CONFIGURATION WIDGET *
 ***********************************/
SimulationConfigurationWidget::SimulationConfigurationWidget(QWidget * parent) :
    QWidget(parent), m_seeding_enabled_cb(new QCheckBox),
    m_seeding_disabled_cb(new QCheckBox), m_seeding_button_group(new QButtonGroup),
    m_slope_sb(new QSpinBox)
{
    setFixedSize(SIMULATION_CONFIGURATION_WIDGET_WIDTH_HEIGHT,SIMULATION_CONFIGURATION_WIDGET_WIDTH_HEIGHT);

    m_seeding_button_group->addButton(m_seeding_enabled_cb);
    m_seeding_button_group->addButton(m_seeding_disabled_cb);

    m_slope_sb->setRange(0,90);

    init_layout();
    reset();
}

SimulationConfigurationWidget::~SimulationConfigurationWidget()
{

}


void SimulationConfigurationWidget::reset()
{
    m_seeding_enabled_cb->setChecked(true);
}

bool SimulationConfigurationWidget::seedingEnabled()
{
    return m_seeding_enabled_cb->isChecked();
}

int SimulationConfigurationWidget::slope()
{
    return m_slope_sb->value();
}

//SimulationOptions SimulationConfigurationWidget::getSimulationConfiguration()
//{
//    return SimulationOptions(m_seeding_enabled_cb->isChecked());
//}

void SimulationConfigurationWidget::init_layout()
{
    QVBoxLayout * main_layout = new QVBoxLayout();

    // SEEDING
    {
        // Title
        QFont title_font( "Arial", 16, QFont::Bold );
        QLabel * seeding_title = new QLabel("Seeding:");
        seeding_title->setFont(title_font);
        main_layout->addWidget(seeding_title, 0, Qt::AlignLeft|Qt::AlignTop);

        // Simplified seeding v1
        QHBoxLayout * seeding_layout = new QHBoxLayout();
        QLabel * seeding_label  = new QLabel("Seeding enabled: ");
        seeding_layout->addWidget(seeding_label, 1, Qt::AlignLeft|Qt::AlignTop);
        seeding_layout->addWidget(m_seeding_enabled_cb,0, Qt::AlignRight|Qt::AlignTop);
        main_layout->addLayout(seeding_layout, 0);

        // Seeding disabled
        QHBoxLayout * seeding_disabled_layout = new QHBoxLayout();
        QLabel * seeding_disabled_label  = new QLabel("Seeding disabled: ");
        seeding_disabled_layout->addWidget(seeding_disabled_label, 1, Qt::AlignLeft|Qt::AlignTop);
        seeding_disabled_layout->addWidget(m_seeding_disabled_cb,0, Qt::AlignRight|Qt::AlignTop);
        main_layout->addLayout(seeding_disabled_layout, 0);
    }
    // SLOPE
    {
        QHBoxLayout * slope_layout(new QHBoxLayout);

        // Title
        {
            QFont title_font( "Arial", 16, QFont::Bold );
            QLabel * slope_title = new QLabel("Slope:");
            slope_title->setFont(title_font);
            slope_layout->addWidget(slope_title);
            slope_layout->addWidget(m_slope_sb);
            slope_layout->addWidget(new QLabel(" Degrees"));
        }

        main_layout->addLayout(slope_layout, 0);
    }

    main_layout->addLayout(new QHBoxLayout,1); // Padding

    setLayout(main_layout);
}
