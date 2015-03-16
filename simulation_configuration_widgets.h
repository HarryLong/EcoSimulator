#ifndef PLANT_CONFIGURATION_WIDGET_H
#define PLANT_CONFIGURATION_WIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QStyledItemDelegate>
#include <set>

#include "constants.h"
#include "simulation_configuration.h"

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
    SimulationConfigurationWidget( int width, int height, QWidget * parent = 0);
    ~SimulationConfigurationWidget();
    SimulationOptions getSimulationConfiguration();

private:
    void init_layout();

    QButtonGroup * m_seeding_button_group;
    QCheckBox * m_per_plant_seeding_cb;
    QCheckBox * m_simplified_seeding_cb;
    QCheckBox * m_seeding_disabled_cb;

    int m_width, m_height;
};

/******************************
 * PLANT CONFIGURATION WIDGET *
 ******************************/

enum AddedPlantsTableColumns {
    Specie,
    Count,
    NumberOfColumns
};

class PlantConfigurationWidget : public QWidget
{
    Q_OBJECT
public:
    PlantConfigurationWidget( int width, int height, QWidget * parent = 0);
    ~PlantConfigurationWidget();
    void reset();
    std::map<int, int> getPlantsToCreate();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private slots:
    void add_specie(QListWidgetItem * item);
    void cell_changed(int p_row, int p_column);

private:
    void init_available_plants_list();
    void init_added_plants_table();
    QTableWidgetItem * generate_read_only_cell(QString p_cell_content = "");
    void init_layout();
    void init_signals();

    QListWidget * m_available_plants_list;
    QTableWidget * m_added_plants_table;
    std::map<QString, int> m_specie_name_to_id_mapper;
    std::set<QString> m_species_added;
    int m_width, m_height;
};

class NumberDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    NumberDelegate(QObject *parent = 0);

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // PLANT_CONFIGURATION_WIDGET_H
