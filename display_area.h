#ifndef DISPLAY_AREA_H
#define DISPLAY_AREA_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <qnamespace.h>
#include <QVector>
#include <QRgb>
#define COLORS Qt::red, Qt::yellow, Qt::green, Qt::white, Qt::blue, Qt::magenta
#define COLOR_COUNT 6

#include <unordered_set>

class Plant;

class DisplayArea : public QWidget
{
    Q_OBJECT

public:
    DisplayArea(int p_width, int p_height, QWidget *parent = 0);
    ~DisplayArea();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void drawPlants(const std::vector<Plant*> & p_incremental_sorted_plants);

    // Test methods
    void drawBoundingBoxes(const std::vector<std::unordered_set<int>*> & p_bounding_box_data);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void init_layout();
    void refresh();

    int m_width, m_height;
    QLabel m_container_label;
    QImage * m_image;
    QPalette m_pal;
    std::vector<Qt::GlobalColor> m_colors;

    // Test method
    QRgb get_random_color();
};

#endif //DISPLAY_AREA_H
