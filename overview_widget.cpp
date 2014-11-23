#include "overview_widget.h"
#include <iostream>
#include <QHeaderView>
#include "debuger.h"

OverViewWidget::OverViewWidget(QWidget *parent)
    : QTableWidget(parent), m_data()
{
    setColumnCount(NumberOfColumns);
    setHorizontalHeaderItem(SpecieColumn,new QTableWidgetItem("Specie"));
    setHorizontalHeaderItem(ColorColumn,new QTableWidgetItem("Color"));
    setHorizontalHeaderItem(CountColumn,new QTableWidgetItem("Count"));
}

OverViewWidget::~OverViewWidget()
{

}

QSize OverViewWidget::minimumSizeHint() const
{
    return QSize(700, 700);
}

QSize OverViewWidget::sizeHint() const
{
    return QSize(700, 700);
}

void OverViewWidget::addPlant(QString specie_name, QColor color)
{
    if(m_data.find(specie_name) == m_data.end()) // Need to add it to the internal data map
    {
        int row_id (rowCount());
        m_data.insert(std::pair<QString, SpecieRow>(specie_name, SpecieRow(row_id, color)));
        insertRow(row_id);
        setItem(row_id, SpecieColumn,  new QTableWidgetItem(specie_name));
        setItem(row_id, ColorColumn, new QTableWidgetItem());
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
    DEBUG_MSG
    setColumnCount(m_column_id+1);
    DEBUG_MSG
    m_causes_of_death_columns.insert(std::pair<QString, int>(name, m_column_id));
    setHorizontalHeaderItem(m_column_id,new QTableWidgetItem(name));
}

void OverViewWidget::refresh(QString specie_name)
{
    SpecieRow row (m_data.find(specie_name)->second);
    int row_id (row.row_id );
    int occurence_count (row.occurence_count);

    setItem(row_id, CountColumn, new QTableWidgetItem(QString::number(occurence_count)));

    // Causes of death
    for(auto it (row.causes_of_death_count.begin()); it != row.causes_of_death_count.end(); it++)
    {
        setItem(row_id, m_causes_of_death_columns[it->first], new QTableWidgetItem(QString::number(it->second)));
    }
}

void OverViewWidget::reset()
{
    m_data.clear();
    while (rowCount() > 0)
        removeRow(0);
}
