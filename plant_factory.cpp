#include "plant_factory.h"
#include <QColor>
#include "plant.h"

PlantFactory::PlantFactory() : m_dice_roller(0,1000), m_specie_properties(PlantDB().getAllPlantData()), m_plant_color_mapper()
{
    for(auto it(m_specie_properties.begin()); it != m_specie_properties.end(); it++)
    {
        m_specie_name_to_id_mapper.insert(std::pair<QString, int>(it->second->specie_name, it->first));
    }
}

PlantFactory::~PlantFactory()
{
    for(auto it (m_specie_properties.begin()); it != m_specie_properties.end(); it++)
        delete it->second;
}

static long unique_id = 1;
Plant* PlantFactory::generate(QString p_specie_name, QPoint p_center_coord)
{
    return generate(get_specie_id(p_specie_name), p_center_coord);
}

Plant* PlantFactory::generate(QString p_specie_name)
{
    return generate(get_specie_id(p_specie_name), generate_random_position());
}

Plant* PlantFactory::generate(int p_specie_id, QPoint p_center_coord)
{
    return new Plant(m_specie_properties.find(p_specie_id)->second,
                     m_plant_color_mapper.getColor(p_specie_id),
                     p_center_coord,
                     unique_id++,
                     m_dice_roller.generate());
}

Plant* PlantFactory::generate(int p_specie_id)
{
    return generate(p_specie_id, generate_random_position());
}

std::vector<QString> PlantFactory::getAllSpecieNames()
{
    std::vector<QString> ret;

    for(auto it (m_specie_properties.begin()); it != m_specie_properties.end(); it++)
        ret.push_back(it->second->specie_name);

    return ret;
}

int PlantFactory::get_specie_id(const QString & name)
{
    return m_specie_name_to_id_mapper.find(name)->second;
}

QPoint PlantFactory::generate_random_position()
{
    return QPoint(rand()%AREA_WIDTH_HEIGHT, rand()%AREA_WIDTH_HEIGHT);
}

SpecieToColorMapper::SpecieToColorMapper() : m_specie_id_to_color(), m_color_stack()
{
    // Initialize the stack
    m_color_stack.push(Qt::white);
    m_color_stack.push(Qt::darkGray);
    m_color_stack.push(Qt::gray);
    m_color_stack.push(Qt::lightGray);
    m_color_stack.push(Qt::red);
    m_color_stack.push(Qt::green);
    m_color_stack.push(Qt::blue);
    m_color_stack.push(Qt::cyan);
    m_color_stack.push(Qt::magenta);
    m_color_stack.push(Qt::yellow);
    m_color_stack.push(Qt::darkRed);
    m_color_stack.push(Qt::darkGreen);
    m_color_stack.push(Qt::darkBlue);
    m_color_stack.push(Qt::darkCyan);
    m_color_stack.push(Qt::darkMagenta);
    m_color_stack.push(Qt::darkYellow);
}

std::shared_ptr<QColor> SpecieToColorMapper::getColor(int p_specie_id)
{
    auto it(m_specie_id_to_color.find(p_specie_id));
    if(it == m_specie_id_to_color.end())
    {
        std::shared_ptr<QColor> color(new QColor(m_color_stack.top()));
        m_color_stack.pop();
        m_specie_id_to_color.insert(std::pair<int, std::shared_ptr<QColor> >(p_specie_id, color));
        return color;
    }

    return it->second;
}
