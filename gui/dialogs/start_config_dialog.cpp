#include "start_config_dialog.h"
#include <QStringListModel>
#include <QGridLayout>
#include <QPushButton>
#include <algorithm>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "monthly_humidity_edit_dlg.h"
#include "monthly_temp_edit_dlg.h"
#include "monthly_illumination_edit_dlg.h"
#include "../widgets/simulation_configuration_widget.h"
#include "../widgets/plant_configuration_widget.h"
//#include "../widgets/input_widgets.h"

#define START_DIALOG_DIALOG_WIDTH 900
#define START_DIALOG_DIALOG_HEIGHT 900

StartConfigDialog::StartConfigDialog ( QWidget * parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    m_widgets()
{
    m_previous_btn = new QPushButton("Previous");
    m_next_start_btn = new QPushButton("Next");

    setWindowTitle(m_widgets.getTitle());
    init_layout();
    init_signals();

    update_previous_button(m_widgets.hasPrevious());
    update_next_button(m_widgets.hasNext());
}

StartConfigDialog::~StartConfigDialog()
{
}

void StartConfigDialog::init_layout()
{
    QGridLayout * main_layout = new QGridLayout;

    QWidget ** all_widgets(m_widgets.getAllWidgets());
    for(int i(0); i < StartConfigDialogWidgets::WidgetType::WidgetCount; i++)
        main_layout->addWidget(all_widgets[i],0,0,1,1,Qt::AlignCenter);

    QHBoxLayout * button_layout = new QHBoxLayout();
    button_layout->addWidget(m_previous_btn, 0 , Qt::AlignCenter);
    button_layout->addWidget(m_next_start_btn, 0 , Qt::AlignCenter);
    main_layout->addLayout(button_layout,1,0,1,1,Qt::AlignCenter);

    setLayout(main_layout);
}

void StartConfigDialog::display_next()
{
    if(m_widgets.hasNext())
    {
        setWindowTitle(m_widgets.displayNext());
        update_previous_button(true);
        update_next_button(m_widgets.hasNext());
    }
    else
        accept();
}

void StartConfigDialog::display_previous()
{
    if(m_widgets.hasPrevious())
    {
        setWindowTitle(m_widgets.displayPrevious());
        update_next_button(true);
        update_previous_button(m_widgets.hasPrevious());
    }
}

void StartConfigDialog::reset()
{
    m_widgets.reset();
    update_next_button(true);
}

void StartConfigDialog::update_previous_button(bool enabled)
{
    m_previous_btn->setEnabled(enabled);
}

void StartConfigDialog::update_next_button(bool enabled)
{
    if(enabled)
        m_next_start_btn->setText("Next");
    else
        m_next_start_btn->setText("Start");
}

void StartConfigDialog::init_signals()
{
    connect(m_previous_btn, SIGNAL(clicked()), this, SLOT(display_previous()));
    connect(m_next_start_btn, SIGNAL(clicked()), this, SLOT(display_next()));
}

SimulationConfiguration StartConfigDialog::getStartConfiguration()
{
    // Humidity
    MonthlyEditDialog * humidity_dlg (static_cast<MonthlyEditDialog*>(m_widgets[StartConfigDialogWidgets::WidgetType::Humidity]));

    // Illumination
    MonthlyEditDialog * illumination_dlg (static_cast<MonthlyEditDialog*>(m_widgets[StartConfigDialogWidgets::WidgetType::Illumination]));

    // Temperature
    MonthlyEditDialog * temp_dlg (static_cast<MonthlyEditDialog*>(m_widgets[StartConfigDialogWidgets::WidgetType::Temperature]));

    bool seeding_enabled(static_cast<SimulationConfigurationWidget*>(m_widgets[StartConfigDialogWidgets::WidgetType::SimulationOptions])->seedingEnabled());

    SimulationConfiguration ret(
                static_cast<PlantConfigurationWidget*>(m_widgets[StartConfigDialogWidgets::WidgetType::PlantConfiguration])->getPlantsToCreate(),
                0, // SLOPE
                humidity_dlg->values(),
                illumination_dlg->values(),
                temp_dlg->values(),
                -1,
                seeding_enabled);

    return ret;
}

QSize StartConfigDialog::minimumSizeHint() const
{
    return QSize(START_DIALOG_DIALOG_WIDTH, START_DIALOG_DIALOG_HEIGHT);
}

QSize StartConfigDialog::sizeHint() const
{
    return minimumSizeHint();
}

/*****************
 * WIDGET HOLDER *
 *****************/
StartConfigDialogWidgets::StartConfigDialogWidgets() : m_current_widget()
{
    // Add the widgets
    m_raw_widgets[StartConfigDialogWidgets::WidgetType::PlantConfiguration] = new PlantConfigurationWidget;
    m_raw_widgets[StartConfigDialogWidgets::WidgetType::Humidity] = new MonthlyHumidityEditDlg;
    m_raw_widgets[StartConfigDialogWidgets::WidgetType::Illumination] = new MonthlyIlluminationEditDlg;
    m_raw_widgets[StartConfigDialogWidgets::WidgetType::Temperature] = new MonthlyTempEditDlg;
    m_raw_widgets[StartConfigDialogWidgets::WidgetType::SimulationOptions] = new SimulationConfigurationWidget;

    // Titles
    // Add the widgets
    m_titles[StartConfigDialogWidgets::WidgetType::PlantConfiguration] = "Plant Configuration";
    m_titles[StartConfigDialogWidgets::WidgetType::Humidity] = "Humidity";
    m_titles[StartConfigDialogWidgets::WidgetType::Illumination] = "Illumination";
    m_titles[StartConfigDialogWidgets::WidgetType::Temperature] = "Temperature";
    m_titles[StartConfigDialogWidgets::WidgetType::SimulationOptions] = "Simulation Options";

    reset();
}

StartConfigDialogWidgets::~StartConfigDialogWidgets()
{
    // All widgets will be inserted into a layout. The layout will delete them
}

void StartConfigDialogWidgets::reset()
{
    static_cast<PlantConfigurationWidget*>(m_raw_widgets[StartConfigDialogWidgets::WidgetType::PlantConfiguration])->reset();
    static_cast<MonthlyEditDialog*>(m_raw_widgets[StartConfigDialogWidgets::WidgetType::Humidity])->reset();
    static_cast<MonthlyEditDialog*>(m_raw_widgets[StartConfigDialogWidgets::WidgetType::Illumination])->reset();
    static_cast<MonthlyEditDialog*>(m_raw_widgets[StartConfigDialogWidgets::WidgetType::Temperature])->reset();
    static_cast<SimulationConfigurationWidget*>(m_raw_widgets[StartConfigDialogWidgets::WidgetType::SimulationOptions])->reset();
    show(StartConfigDialogWidgets::WidgetType::PlantConfiguration);
}

void StartConfigDialogWidgets::show(WidgetType p_type)
{
    hide_all();
    m_raw_widgets[p_type]->show();
    m_current_widget = p_type;
}

void StartConfigDialogWidgets::hide(WidgetType p_type)
{
    m_raw_widgets[p_type]->hide();
}

void StartConfigDialogWidgets::hide_all()
{
    for(int i(0); i < StartConfigDialogWidgets::WidgetType::WidgetCount; i++)
    {
        m_raw_widgets[i]->hide();
    }
}

bool StartConfigDialogWidgets::hasPrevious()
{
    return m_current_widget != 0;
}

bool StartConfigDialogWidgets::hasNext()
{
    return m_current_widget != (StartConfigDialogWidgets::WidgetType::WidgetCount-1);
}

QString StartConfigDialogWidgets::displayPrevious()
{
    if(hasPrevious())
    {
        show(static_cast<WidgetType>(static_cast<int>(m_current_widget)-1));
    }
    return m_titles[m_current_widget];
}

QString StartConfigDialogWidgets::displayNext()
{
    if(hasNext())
    {
        show(static_cast<WidgetType>(static_cast<int>(m_current_widget)+1));
    }
    return m_titles[m_current_widget];
}

QString StartConfigDialogWidgets::getTitle()
{
    return m_titles[m_current_widget];
}

QWidget** StartConfigDialogWidgets::getAllWidgets()
{
    return m_raw_widgets;
}

QWidget* StartConfigDialogWidgets::operator[](WidgetType p_type)
{
    return m_raw_widgets[p_type];
}

