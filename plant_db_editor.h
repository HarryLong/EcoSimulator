#ifndef PLANT_DB_EDITOR_H
#define PLANT_DB_EDITOR_H

#define PLANT_DB_EDITOR_DIALOG_WIDTH 700
#define PLANT_DB_EDITOR_DIALOG_HEIGHT 700

#include <QDialog>
#include <QListWidget>
#include <QDoubleSpinBox>
#include <QTextEdit>

#include "plant_db.h"

class GrowthPropertiesWidget;
class AgeingPropertiesWidget;
class IlluminationPropertiesWidget;
class SoilHumidityPropertiesWidget;

struct PropertyWidgets{
    GrowthPropertiesWidget * growth_property_widget;
    AgeingPropertiesWidget * ageing_properties_widget;
    IlluminationPropertiesWidget * illumination_properties_widget;
    SoilHumidityPropertiesWidget * soil_humidity_properties_widget;

    PropertyWidgets(GrowthPropertiesWidget * growth_property_widget,
                    AgeingPropertiesWidget * ageing_properties_widget,
                    IlluminationPropertiesWidget * illumination_properties_widget,
                    SoilHumidityPropertiesWidget * soil_humidity_properties_widget
                    ) :
        growth_property_widget(growth_property_widget),
        ageing_properties_widget(ageing_properties_widget),
        illumination_properties_widget(illumination_properties_widget),
        soil_humidity_properties_widget(soil_humidity_properties_widget) {}
};

class PlantDBEditor : public QDialog
{
Q_OBJECT
public:
    PlantDBEditor ( QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~PlantDBEditor();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private slots:
    void edit_btn_clicked();
    void cancel_btn_clicked();
    void new_btn_clicked();
    void update_widgets(QListWidgetItem* p_selected_list_item);

private:
    void init_layout();
    void init_content();
    void set_widgets_enabled(bool enabled);
    void init_signals();

    QListWidget * m_available_plants_list;
    PlantDB m_plant_db;

    QTextEdit * m_specie_name_te;
    PlantDataHolder m_plant_data;
    PropertyWidgets m_property_widgets;

    QPushButton * m_edit_confirm_btn;
    QPushButton * m_cancel_btn;
    QPushButton * m_new_btn;

    bool m_edit_mode_on;
};

/******************
 * HELPER CLASSES *
 ******************/
class MyCustomDoubleSpinBox : public QDoubleSpinBox
{
public:
    MyCustomDoubleSpinBox() : QDoubleSpinBox()
    {
        setRange(.0,1000.0);
        setDecimals(2);
    }
};

class MyPercentageSpinBox : public QSpinBox
{
public:
    MyPercentageSpinBox() : QSpinBox()
    {
        setRange(0,100);
    }
};

/****************************
 * GROWTH PROPERTIES WIDGET *
 ****************************/
class GrowthPropertiesWidget : public QWidget
{
public:
    GrowthPropertiesWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~GrowthPropertiesWidget();

    GrowthProperties getProperties();

    void setProperties(GrowthProperties p_properties);
    void setEditable(bool p_editable);

private:
    void init_layout();

    // UI Elements
    MyCustomDoubleSpinBox* m_max_vertical_growth_dsb;
    MyCustomDoubleSpinBox* m_max_canopy_growth_dsb;
    MyCustomDoubleSpinBox* m_max_roots_growth_dsb;
};

/****************************
 * AGEING PROPERTIES WIDGET *
 ****************************/
class AgeingPropertiesWidget : public QWidget
{
public:
    AgeingPropertiesWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~AgeingPropertiesWidget();

    AgeingProperties getProperties();

    void setProperties(AgeingProperties p_properties);

private:
    void init_layout();

    // UI Elements
    MyPercentageSpinBox * m_prob_of_death_at_birth;
    MyPercentageSpinBox * m_prob_of_death_at_upper;

    QSpinBox* m_prime_age_start_sb;
    QSpinBox* m_prime_age_end_sb;
    QSpinBox* m_upper_age_sb;
};

/**********************************
 * ILLUMINATION PROPERTIES WIDGET *
 **********************************/
class IlluminationPropertiesWidget : public QWidget
{
public:
    IlluminationPropertiesWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~IlluminationPropertiesWidget();

    IlluminationProperties getProperties();

    void setProperties(IlluminationProperties p_properties);

private:
    void init_layout();

    // UI Elements
    MyPercentageSpinBox* m_shadowed_percentage_start_of_negative_impact_sb;
    MyPercentageSpinBox* m_prob_of_death_in_complete_shade_sb;
};

/***********************************
 * SOIL HUMIDITY PROPERTIES WIDGET *
 ***********************************/
class SoilHumidityPropertiesWidget : public QWidget
{
public:
    SoilHumidityPropertiesWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~SoilHumidityPropertiesWidget();

    SoilHumidityProperties getProperties();

    void setProperties(SoilHumidityProperties p_properties);

private:
    void init_layout();

    // UI Elements
    MyPercentageSpinBox* m_prime_soil_humidity_percentage_start_sb;
    MyPercentageSpinBox* m_prime_soil_humidity_percentage_end_sb;
};

#endif //PLANT_DB_EDITOR_H
