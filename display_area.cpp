#include "display_area.h"
#include "constants.h"

#include <stdlib.h>
#include <initializer_list>
#include <iostream>
#include <QSize>
#include <QGridLayout>
#include "plant.h"
#include <random>

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

void DisplayArea::drawPlants(const std::vector<Plant*> & p_incremental_sorted_plants)
{
    std::cout << "Drawing " << p_incremental_sorted_plants.size() << " plants." << std::endl;
    for(auto it = p_incremental_sorted_plants.rbegin(); it != p_incremental_sorted_plants.rend(); it++)
    {

    }
}

void DisplayArea::drawBoundingBoxes(const std::vector<std::unordered_set<int>*> & p_bounding_box_data)
{
    static Qt::GlobalColor all_colors[] = {COLORS};
    static std::map<int, QRgb> plant_to_color_map;

    QImage image(m_width, m_height, QImage::Format::Format_RGB32);
    for(int x = 0; x < m_width; x++)
    {
        for(int y = 0; y < m_width; y++)
        {
            int index((y*m_width) + x);
            if(!p_bounding_box_data[index]->empty())
            {
                int plant_id(*(p_bounding_box_data[index]->begin()));
                if(plant_to_color_map.find(plant_id) == plant_to_color_map.end())
                    plant_to_color_map[plant_id] = QColor(all_colors[rand() % (COLOR_COUNT-1)]).rgb();

                std::cout << "Setting pixel [" << x << ", " << y << "to color: " << QColor(plant_to_color_map[plant_id]).rgb() << std::endl;
                image.setPixel(x, y, plant_to_color_map[plant_id]);
//                std::cout << "Setting pixel [" << x << ", " << y << "to color: " << QColor(Qt::red).rgb() << std::endl;
//                image.setPixel(x, y, QColor(Qt::red).rgb());
            }
        }
    }

    m_container_label.setPixmap(QPixmap::fromImage(image));
}
