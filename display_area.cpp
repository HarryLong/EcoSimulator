#include "display_area.h"
#include "constants.h"

#include <stdlib.h>
#include <initializer_list>
#include <iostream>
#include <QSize>
#include <QGridLayout>

DisplayArea::DisplayArea(int p_width, int p_height, QWidget *parent)
    : QWidget(parent), m_container_label(), m_width(p_width), m_height(p_height),
      m_pal(palette()), m_colors(std::initializer_list<Qt::GlobalColor>{COLORS})
{
    m_image = new QImage(m_width, m_height, QImage::Format::Format_RGB32);
    m_image->fill(Qt::black);
    init_layout();
    refresh();
}

DisplayArea::~DisplayArea()
{

}

QSize DisplayArea::minimumSizeHint() const
{
    return QSize(m_width, m_height);
}

QSize DisplayArea::sizeHint() const
{
    return QSize(m_width+100, m_height+100);
}

void DisplayArea::paintEvent(QPaintEvent *event)
{
}

void DisplayArea::setData(uchar* p_image_data)
{
    m_image = new QImage(p_image_data, m_width, m_height, QImage::Format::Format_RGB32);
    refresh();
}

//uchar * DisplayArea::getArena()
//{
//    return m_image.bits();

//    std::cout << "Setting pixel" << std::endl;

//    int random_pixel_x = rand() % AREA_WIDTH;
//    int random_pixel_y = rand() % AREA_HEIGHT;

//    m_image.setPixel(random_pixel_x, random_pixel_y, Qt::white);
//    refresh();

//    Qt::GlobalColor color = m_colors[rand() % m_colors.size()];
//    std::cout << "Chosen color: " << color << std::endl;
//    m_pal.setColor(QPalette::Background, color);
//    setAutoFillBackground(true);
//    setPalette(m_pal);
//}

void DisplayArea::refresh()
{
    m_container_label.setPixmap(QPixmap::fromImage(*m_image));
}

void DisplayArea::init_layout()
{
    QGridLayout * main_layout = new QGridLayout;
    main_layout->addWidget(&m_container_label,0,0,1,1, Qt::AlignCenter);
    setLayout(main_layout);
}
