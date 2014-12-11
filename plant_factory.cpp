#include "plant_factory.h"
#include <QColor>
#include "plant.h"

PlantFactory::PlantFactory() : m_dice_roller(0,1000), m_specie_properties(PlantDB().getAllPlantData()), m_plant_color_mapper()
{
}

PlantFactory::~PlantFactory()
{
    for(auto it (m_specie_properties.begin()); it != m_specie_properties.end(); it++)
        delete it->second;
}

static long unique_id = 1;
Plant* PlantFactory::generate(QString p_specie_name, QPoint p_center_coord)
{
    return new Plant(m_specie_properties.find(p_specie_name)->second,
                     m_plant_color_mapper.getColor(p_specie_name),
                     p_center_coord,
                     unique_id++,
                     m_dice_roller.generate());
}

std::vector<QString> PlantFactory::getAllSpecieNames()
{
    std::vector<QString> ret;

    for(auto it (m_specie_properties.begin()); it != m_specie_properties.end(); it++)
        ret.push_back(it->first);

    return ret;
}

SpecieToColorMapper::SpecieToColorMapper() : m_specie_to_color(), m_color_stack()
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

std::shared_ptr<QColor> SpecieToColorMapper::getColor(QString p_specie_name)
{
    auto it(m_specie_to_color.find(p_specie_name));
    if(it == m_specie_to_color.end())
    {
        std::shared_ptr<QColor> color(new QColor(m_color_stack.top()));
        m_color_stack.pop();
        m_specie_to_color.insert(std::pair<QString, std::shared_ptr<QColor> >(p_specie_name, color));
        return color;
    }

    return it->second;
}

