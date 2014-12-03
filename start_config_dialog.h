#ifndef START_CONFIG_DIALOG_H
#define START_CONFIG_DIALOG_H

#define DIALOG_WIDTH 800
#define DIALOG_HEIGHT 800
#define WINDOW_TITLE "Starting Configuration"

#include <QDialog>

#include "plant_factory.h"
#include "plant_configuration_widget.h"
#include "input_widgets.h"
#include <map>

class QPushButton;

enum WidgetType {
    PlantConfiguration = 0,
    SoilHumidityConfiguration,
    IlluminationConfiguration,

    WidgetCount
};

struct StartConfiguration {
    std::vector<Plant*> m_plants;
    const QImage * soil_humidity;
    const QImage * illumination;
};

/*****************
 * WIDGET HOLDER *
 *****************/
class StartConfigDialogWidgets{
public:
    StartConfigDialogWidgets();
    ~StartConfigDialogWidgets();

    void add(WidgetType p_type, QWidget* p_widget);
    void resetAll();
    bool hasNext();
    bool hasPrevious();
    void displayPrevious();
    void displayNext();
    QWidget* get(WidgetType p_type);
    std::vector<QWidget*> getAllWidgets();

private:
    void show(WidgetType p_type);
    void hide(WidgetType p_type);
    void hide_all();
    std::map<WidgetType, QWidget*> m_widgets;
    WidgetType m_current_widget;
    int m_current_widget_index;
};

class StartConfigDialog : public QDialog
{
Q_OBJECT
public:
    StartConfigDialog ( QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~StartConfigDialog();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void reset();

    StartConfiguration getStartConfiguration();

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
