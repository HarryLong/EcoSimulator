#include "start_config_dialog.h"
#include <QStringListModel>
#include <QGridLayout>
#include <QPushButton>
#include "constants.h"
#include <algorithm>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

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

    for(QWidget * widget : m_widgets.getAllWidgets())
        main_layout->addWidget(widget,0,0,1,1,Qt::AlignCenter);

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

StartConfiguration StartConfigDialog::getStartConfiguration()
{
    StartConfiguration ret(
                static_cast<PlantConfigurationWidget*>(m_widgets.get(WidgetType::PlantConfiguration))->getPlantsToCreate(),
                static_cast<InputWidget*>(m_widgets.get(WidgetType::SoilHumidityConfiguration))->getData(),
                static_cast<InputWidget*>(m_widgets.get(WidgetType::IlluminationConfiguration))->getData());

    return ret;
}

QSize StartConfigDialog::minimumSizeHint() const
{
    return QSize(RENDER_WINDOW_WIDTH_HEIGHT+200, RENDER_WINDOW_WIDTH_HEIGHT+200);
}

QSize StartConfigDialog::sizeHint() const
{
    return minimumSizeHint();
}

QPoint StartConfigDialog::generate_random_position()
{
    return QPoint(rand()%AREA_WIDTH_HEIGHT, rand()%AREA_WIDTH_HEIGHT);
}

/*****************
 * WIDGET HOLDER *
 *****************/
StartConfigDialogWidgets::StartConfigDialogWidgets() : m_widgets(), m_current_widget(WidgetType::PlantConfiguration), m_current_widget_index(0)
{
    // Add the widgets
    m_widgets.insert(std::pair<WidgetType, QWidget*>(WidgetType::PlantConfiguration, new PlantConfigurationWidget(RENDER_WINDOW_WIDTH_HEIGHT, RENDER_WINDOW_WIDTH_HEIGHT)));
    m_widgets.insert(std::pair<WidgetType, QWidget*>(WidgetType::SoilHumidityConfiguration, new SoilHumidityInputWidget(RENDER_WINDOW_WIDTH_HEIGHT, RENDER_WINDOW_WIDTH_HEIGHT)));
    m_widgets.insert(std::pair<WidgetType, QWidget*>(WidgetType::IlluminationConfiguration, new IlluminationInputWidget(RENDER_WINDOW_WIDTH_HEIGHT, RENDER_WINDOW_WIDTH_HEIGHT)));

    // Title
    m_titles.insert(std::pair<WidgetType, QString>(WidgetType::PlantConfiguration, QString("Plsnt Configuration")));
    m_titles.insert(std::pair<WidgetType, QString>(WidgetType::SoilHumidityConfiguration, QString("Soil Humidity")));
    m_titles.insert(std::pair<WidgetType, QString>(WidgetType::IlluminationConfiguration, QString("Illumination")));

    hide_all();
    show(m_current_widget);
}

StartConfigDialogWidgets::~StartConfigDialogWidgets()
{
    // All widgets will be inserted into a layout. The layout will delete them
}

void StartConfigDialogWidgets::show(WidgetType p_type)
{
    m_widgets.find(p_type)->second->show();
}

void StartConfigDialogWidgets::hide(WidgetType p_type)
{
    m_widgets.find(p_type)->second->hide();
}

void StartConfigDialogWidgets::hide_all()
{
    for(auto it (m_widgets.begin()); it != m_widgets.end(); it++)
    {
        it->second->hide();
    }
}

bool StartConfigDialogWidgets::hasPrevious()
{
    return m_current_widget != 0;
}

bool StartConfigDialogWidgets::hasNext()
{
    return m_current_widget != (WidgetType::WidgetCount-1);
}

QString StartConfigDialogWidgets::displayPrevious()
{
    if(hasPrevious())
    {
        hide(m_current_widget);
        m_current_widget = static_cast<WidgetType>(--m_current_widget_index);
        show(m_current_widget);
    }
    return m_titles.find(m_current_widget)->second;
}

QString StartConfigDialogWidgets::displayNext()
{
    if(hasNext())
    {
        hide(m_current_widget);
        m_current_widget = static_cast<WidgetType>(++m_current_widget_index);
        show(m_current_widget);
    }
    return m_titles.find(m_current_widget)->second;
}

QString StartConfigDialogWidgets::getTitle()
{
    return m_titles.find(m_current_widget)->second;
}

std::vector<QWidget*> StartConfigDialogWidgets::getAllWidgets()
{
    std::vector<QWidget*> ret;
    for(auto it (m_widgets.begin()); it != m_widgets.end(); it++)
        ret.push_back(it->second);

    return ret;
}

QWidget* StartConfigDialogWidgets::get(WidgetType p_type)
{
    return m_widgets.find(p_type)->second;
}

