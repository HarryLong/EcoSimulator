#include "plant_factory.h"
#include "plantDB/plant_db.h"

#include <QColor>

const std::vector<QColor> PlantFactory::_COLORS = PlantFactory::get_specie_colors();
int PlantFactory::color_index = 0;
std::map<int, int> PlantFactory::specie_id_to_color_index = std::map<int, int>();
PlantFactory::PlantFactory(int area_width, int area_height) : m_dice_roller(0,1000), m_specie_properties(PlantDB().getAllPlantData()),
    m_area_width(area_width), m_area_height(area_height)
{
    for(auto it(m_specie_properties.begin()); it != m_specie_properties.end(); it++)
    {
        m_specie_name_to_id_mapper.emplace(it->second.specie_name, it->first);
    }
}

PlantFactory::~PlantFactory()
{

}

static long unique_id = 1;
Plant PlantFactory::generate(QString p_specie_name, QPoint p_center_coord)
{
    return generate(get_specie_id(p_specie_name), p_center_coord);
}

Plant PlantFactory::generate(QString p_specie_name)
{
    return generate(get_specie_id(p_specie_name), generate_random_position());
}

Plant PlantFactory::generate(int p_specie_id, QPoint p_center_coord)
{
    if(specie_id_to_color_index.find(p_specie_id) == specie_id_to_color_index.end())
        specie_id_to_color_index.emplace(p_specie_id, color_index++);

    QColor color(_COLORS.at(specie_id_to_color_index[p_specie_id]));
    return Plant(m_specie_properties.find(p_specie_id)->second,
                 color,
                 p_center_coord,
                 unique_id++,
                 m_dice_roller.generate());
}

Plant PlantFactory::generate(int p_specie_id)
{
    return generate(p_specie_id, generate_random_position());
}

std::vector<QString> PlantFactory::getAllSpecieNames()
{
    std::vector<QString> ret;

    for(auto it (m_specie_properties.begin()); it != m_specie_properties.end(); it++)
        ret.push_back(it->second.specie_name);

    return ret;
}

int PlantFactory::get_specie_id(const QString & name)
{
    return m_specie_name_to_id_mapper.find(name)->second;
}

QPoint PlantFactory::generate_random_position()
{
    return QPoint(rand()%(m_area_width-1), rand()%(m_area_height-1));
}

const SpecieProperties & PlantFactory::getSpecieProperties(int p_specie_id)
{
    return m_specie_properties.find(p_specie_id)->second;
}

std::vector<QColor> PlantFactory::get_specie_colors()
{
    std::vector<QColor> ret;
    ret.push_back(Qt::white);
    ret.push_back(Qt::red);
    ret.push_back(Qt::green);
    ret.push_back(Qt::blue);
    ret.push_back(Qt::cyan);
    ret.push_back(Qt::magenta);
    ret.push_back(Qt::yellow);
    ret.push_back(Qt::darkRed);
    ret.push_back(Qt::darkGreen);
    ret.push_back(Qt::darkBlue);
    ret.push_back(Qt::darkCyan);
    ret.push_back(Qt::darkMagenta);
    ret.push_back(Qt::darkYellow);

    return ret;
}
