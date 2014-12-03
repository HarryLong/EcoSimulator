#ifndef PLANT_CONFIGURATION_WIDGET_H
#define PLANT_CONFIGURATION_WIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QStyledItemDelegate>
#include <set>

#include "plant_factory.h"
#include "constants.h"

class QPushButton;
class QTableWidget;

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
    std::vector<Plant*> getPlantsToCreate();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private slots:
    void add_specie(QListWidgetItem * item);
    void cell_changed(int p_row, int p_column);

private:
    void init_available_plants_list();
    void init_added_plants_table();
    QTableWidgetItem * generate_read_only_cell(QString p_cell_content = "");
    QPoint generate_random_position();
    void init_layout();
    void init_signals();

    QListWidget * m_available_plants_list;
    QTableWidget * m_added_plants_table;
    PlantFactory m_plant_factory;
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
