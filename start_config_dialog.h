#ifndef START_CONFIG_DIALOG_H
#define START_CONFIG_DIALOG_H

#define DIALOG_WIDTH 800
#define DIALOG_HEIGHT 800

#include <QDialog>

#include "plant_factory.h"
#include "simulation_configuration_widgets.h"
#include "input_widgets.h"
#include <map>

class QPushButton;

enum WidgetType {
    PlantConfiguration = 0,
    SoilHumidityConfiguration,
    IlluminationConfiguration,
    SimulationOptions,

    WidgetCount
};

/*****************
 * WIDGET HOLDER *
 *****************/
class StartConfigDialogWidgets{
public:
    StartConfigDialogWidgets();
    ~StartConfigDialogWidgets();

    void add(WidgetType p_type, QWidget* p_widget);
    bool hasNext();
    bool hasPrevious();
    QString getTitle();
    QString displayPrevious();
    QString displayNext();
    QWidget* get(WidgetType p_type);
    std::vector<QWidget*> getAllWidgets();

private:
    void show(WidgetType p_type);
    void hide(WidgetType p_type);
    void hide_all();
    std::map<WidgetType, QWidget*> m_widgets;
    WidgetType m_current_widget;
    int m_current_widget_index;

    std::map<WidgetType, QString> m_titles;
};

class StartConfigDialog : public QDialog
{
Q_OBJECT
public:
    StartConfigDialog ( QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~StartConfigDialog();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    SimulationConfiguration getStartConfiguration();

private slots:
    void display_next();
    void display_previous();

private:
    void init_layout();
    void init_signals();
    void update_previous_button(bool enabled);
    void update_next_button(bool enabled);
    QPoint generate_random_position();

    QPushButton * m_previous_btn;
    QPushButton * m_next_start_btn;

    StartConfigDialogWidgets m_widgets;
};

#endif // START_CONFIG_DIALOG_H
