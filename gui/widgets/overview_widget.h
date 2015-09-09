#ifndef OVERVIEW_WIDGET_H
#define OVERVIEW_WIDGET_H

#include <QSize>
#include <QTableWidget>
#include <string>
#include <map>
#include <QObject>
#include <QStyledItemDelegate>

enum OverViewTableColumns{
    SpecieColumn = 0,
    RenderColumn,
    ColorColumn,
    CountColumn,
    OverViewTableColumnCount
};

struct SpecieRow{
    int occurence_count;
    int row_id;

    std::map<QString, int> causes_of_death_count;
    SpecieRow(int row_id) : occurence_count(0), causes_of_death_count(), row_id(row_id) {}
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

private slots:
    void cell_changed(int row, int column);

signals:
    void filter(QString plant_name);
    void unfilter(QString plant_name);

protected:
//    virtual void init_layout();

private:
    QTableWidgetItem * generate_read_only_cell(QString p_cell_content = "");
    void init_signals();

    QStringList labels;
    std::map<QString,SpecieRow> m_data;
    std::map<QString, int> m_causes_of_death_columns;
    void refresh(QString specie);
    void add_cause_of_death_column(QString name);
};

#endif //OVERVIEW_WIDGET_H
