#include "overview_widget.h"
#include <iostream>
#include <QHeaderView>
#include "debuger.h"
#include "constants.h"

OverViewWidget::OverViewWidget(QWidget *parent)
    : QTableWidget(parent), m_data()
{
    setColumnCount(OverViewTableColumnCount);
    setHorizontalHeaderItem(SpecieColumn,new QTableWidgetItem("Specie"));
    setHorizontalHeaderItem(RenderColumn,new QTableWidgetItem("Render"));
    setHorizontalHeaderItem(ColorColumn,new QTableWidgetItem("Color"));
    setHorizontalHeaderItem(CountColumn,new QTableWidgetItem("Count"));

    init_signals();
}

OverViewWidget::~OverViewWidget()
{

}

QSize OverViewWidget::minimumSizeHint() const
{
    return QSize(OVERVIEW_WIDGET_WINDOW_WIDTH_HEIGHT,
                 OVERVIEW_WIDGET_WINDOW_WIDTH_HEIGHT);
}

QSize OverViewWidget::sizeHint() const
{
    return QSize(OVERVIEW_WIDGET_WINDOW_WIDTH_HEIGHT,
                 OVERVIEW_WIDGET_WINDOW_WIDTH_HEIGHT);
}

void OverViewWidget::init_signals()
{
    connect(this, SIGNAL( cellChanged(int,int)), this, SLOT(cell_changed(int,int)));
}

void OverViewWidget::cell_changed(int row, int column)
{
    if(column == OverViewTableColumns::RenderColumn)
    {
        QString plant_name (item(row,OverViewTableColumns::SpecieColumn)->text());
        if(item(row,OverViewTableColumns::RenderColumn)->checkState() == Qt::Checked)
            emit unfilter(plant_name);
        else
            emit filter(plant_name);
    }
}

void OverViewWidget::addPlant(QString specie_name, QColor color)
{
    if(m_data.find(specie_name) == m_data.end())
    {
        int row_id (rowCount());
        m_data.insert(std::pair<QString, SpecieRow>(specie_name, SpecieRow(row_id, color)));
        insertRow(row_id);

        // Specie column
        setItem(row_id, SpecieColumn,  generate_read_only_cell(specie_name));

        // Render column
        QTableWidgetItem* render_cb_item = new QTableWidgetItem();
        render_cb_item->data(Qt::CheckStateRole);
        render_cb_item->setCheckState(Qt::Checked);
        setItem(row_id, RenderColumn, render_cb_item);

        // Color column
        setItem(row_id, ColorColumn,  generate_read_only_cell());
        item(row_id, ColorColumn)->setBackgroundColor(color);
    }
    m_data.find(specie_name)->second.occurence_count++;
    refresh(specie_name);
}

void OverViewWidget::removePlant(QString specie_name, QString cause_of_death)
{
    auto row_it ( m_data.find(specie_name) );

    if(m_causes_of_death_columns.find(cause_of_death) == m_causes_of_death_columns.end())
        add_cause_of_death_column(cause_of_death);

    // Decrement count
    row_it->second.occurence_count--;

    // Increment cause of death
    auto cod_it(row_it->second.causes_of_death_count.find(cause_of_death));
    if(cod_it == row_it->second.causes_of_death_count.end())
        row_it->second.causes_of_death_count.insert(std::pair<QString, int>(cause_of_death, 1 ));
    else
        cod_it->second++;

    refresh(specie_name);
}

void OverViewWidget::add_cause_of_death_column(QString name)
{
    int m_column_id (columnCount());
    setColumnCount(m_column_id+1);
    m_causes_of_death_columns.insert(std::pair<QString, int>(name, m_column_id));
    setHorizontalHeaderItem(m_column_id,new QTableWidgetItem(name));
}

void OverViewWidget::refresh(QString specie_name)
{
    SpecieRow row (m_data.find(specie_name)->second);
    int row_id (row.row_id );
    int occurence_count (row.occurence_count);

    setItem(row_id, CountColumn, generate_read_only_cell(QString::number(occurence_count)));

    // Causes of death
    for(auto it (row.causes_of_death_count.begin()); it != row.causes_of_death_count.end(); it++)
    {
        setItem(row_id, m_causes_of_death_columns[it->first], generate_read_only_cell(QString::number(it->second)));
    }
}

void OverViewWidget::reset()
{
    m_data.clear();
    while (rowCount() > 0)
        removeRow(0);
}

QTableWidgetItem * OverViewWidget::generate_read_only_cell(QString p_cell_content)
{
    QTableWidgetItem * cell = new QTableWidgetItem(p_cell_content);
    cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);

    return cell;
}
