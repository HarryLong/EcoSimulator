#ifndef PLANT_FACTORY_H
#define PLANT_FACTORY_H

#include <map>
#include "growth_manager.h"
#include <string>
#include <QColor>
#include "dice_roller.h"
#include <memory>
#include "helper.h"
#include "plant_db.h"
#include <stack>

class Plant;

class SpecieToColorMapper {
public:
    SpecieToColorMapper();
    std::shared_ptr<QColor> getColor(QString p_specie_name);

private:
    std::map<QString, std::shared_ptr<QColor>> m_specie_to_color;
    std::stack<QColor> m_color_stack;
};


class PlantFactory {
public:
    PlantFactory();
    ~PlantFactory();
    Plant* generate(QString p_specie_name, QPoint p_center_coord);
    std::vector<QString> getAllSpecieNames();

private:
    SpeciePropertiesHolder m_specie_properties;
    DiceRoller m_dice_roller;
    SpecieToColorMapper m_plant_color_mapper;
};

#endif // PLANT_FACTORY_H
