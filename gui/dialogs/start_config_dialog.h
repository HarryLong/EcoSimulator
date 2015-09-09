#ifndef START_CONFIG_DIALOG_H
#define START_CONFIG_DIALOG_H

#define DIALOG_WIDTH 800
#define DIALOG_HEIGHT 800

#include "../../simulator/core/simulation_configuration.h"

#include <QDialog>
#include <map>

class QPushButton;
class QWidget;

/*****************
 * WIDGET HOLDER *
 *****************/
class StartConfigDialogWidgets{
public:
    enum WidgetType {
        PlantConfiguration = 0,
        Humidity,
        Temperature,
        Illumination,
        SimulationOptions,

        WidgetCount
    };

    StartConfigDialogWidgets();
    ~StartConfigDialogWidgets();

    void add(WidgetType p_type, QWidget* p_widget);
    bool hasNext();
    bool hasPrevious();
    QString getTitle();
    QString displayPrevious();
    QString displayNext();
    QWidget* operator[](WidgetType p_type);
    QWidget** getAllWidgets();
    void reset();

private:
    void show(WidgetType p_type);
    void hide(WidgetType p_type);
    void hide_all();
//    std::map<WidgetType, QWidget*> m_widgets;
    WidgetType m_current_widget;
    QWidget * m_raw_widgets[WidgetType::WidgetCount];
    QString m_titles[WidgetType::WidgetCount];
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

public slots:
    void reset();

private slots:
    void display_next();
    void display_previous();

private:
    void init_layout();
    void init_signals();
    void update_previous_button(bool enabled);
    void update_next_button(bool enabled);

    QPushButton * m_previous_btn;
    QPushButton * m_next_start_btn;

    StartConfigDialogWidgets m_widgets;
};

#endif // START_CONFIG_DIALOG_H
