#ifndef PLANT_DB_EDITOR_H
#define PLANT_DB_EDITOR_H

#define PLANT_DB_EDITOR_DIALOG_WIDTH 1000
#define PLANT_DB_EDITOR_DIALOG_HEIGHT 1000

#include <QDialog>
#include <QListWidget>
#include "plant_db.h"

#include "properties_editor_widgets.h"

class PropertyWidgetsWrapper : public QWidget{
public:
    PropertyWidgetsWrapper();
    void setProperties(const SpecieProperties * p_plant_data);
    void setEnabled(bool p_enabled);
    void clear();
    SpecieProperties * toProperties();

private:
    void init_layout();
    QLineEdit * m_specie_name_widget;
    GrowthPropertiesWidget * growth_property_widget;
    AgeingPropertiesWidget * ageing_properties_widget;
    IlluminationPropertiesWidget * illumination_properties_widget;
    SoilHumidityPropertiesWidget * soil_humidity_properties_widget;
};

enum Mode{
    READ_ONLY,
    EDITING,
    ADDING
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
    void remove_btn_clicked();
    void refresh_property_widgets();

private:
    void init_layout();
    void init_content();
    void set_property_widgets_enabled(bool p_enabled);
    void init_signals();
    void commit(bool p_update);
    void set_mode(Mode p_mode);
    QListWidgetItem* get_current_selected_specie();

    QListWidget * m_available_plants_list;
    PlantDB m_plant_db;

    SpeciePropertiesHolder m_plant_data;
    PropertyWidgetsWrapper * m_property_widgets_wrapper;

    QPushButton * m_edit_save_edits_btn;
    QPushButton * m_cancel_btn;
    QPushButton * m_new_confirm_btn;
    QPushButton * m_remove_btn;

    Mode m_current_mode;
};
#endif //PLANT_DB_EDITOR_H
