#ifndef OVERVIEW_WIDGET_H
#define OVERVIEW_WIDGET_H

#include <QSize>
#include <QTableWidget>
#include <string>
#include <map>

class OverViewWidget : public QTableWidget
{
public:
    OverViewWidget(int p_width, int p_height, QWidget *parent = 0);
    virtual ~OverViewWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
    void addPlant(std::string name, QColor color);
    void removePlant(std::string name, QColor color);

protected:
    virtual void init_layout() = 0;
    int m_width, m_height;
//    std
};

#endif //OVERVIEW_WIDGET_H
