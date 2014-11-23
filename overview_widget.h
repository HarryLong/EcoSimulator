#ifndef OVERVIEW_WIDGET_H
#define OVERVIEW_WIDGET_H

#include <QSize>
#include <QTableWidget>
#include <string>
#include <map>
#include <QObject>
#include "helper.h"

enum Columns{
    SpecieColumn = 0,
    ColorColumn,
    CountColumn,
    NumberOfColumns
};

struct SpecieRow{
    int occurence_count;
    QColor color;
    int row_id;

    std::map<QString, int> causes_of_death_count;
    SpecieRow(int row_id, QColor color) : occurence_count(0), color(color), causes_of_death_count(), row_id(row_id) {}
};

class OverViewWidget : public QTableWidget
{
Q_OBJECT
public:
    OverViewWidget(QWidget *parent = 0);
    virtual ~OverViewWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
    void addPlant(QString specie_name, QColor color);
    void removePlant(QString specie_name, QString cause_of_death);
    void reset();

protected:
//    virtual void init_layout();

private:
    QStringList labels;
    std::map<QString,SpecieRow> m_data;
    std::map<QString, int> m_causes_of_death_columns;
    void refresh(QString specie);
    void add_cause_of_death_column(QString name);
};

#endif //OVERVIEW_WIDGET_H
