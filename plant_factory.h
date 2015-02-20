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
    std::shared_ptr<QColor> getColor(int p_specie_id);

private:
    std::map<int, std::shared_ptr<QColor>> m_specie_id_to_color;
    std::stack<QColor> m_color_stack;
};


class PlantFactory {
public:
    PlantFactory();
    ~PlantFactory();
    Plant* generate(QString p_specie_name, QPoint p_center_coord);
    Plant* generate(QString p_specie_name);
    Plant* generate(int p_specie_id, QPoint p_center_coord);
    Plant* generate(int p_specie_id);
    std::vector<QString> getAllSpecieNames();

private:
    int get_specie_id(const QString & name);
    QPoint generate_random_position();

    SpeciePropertiesHolder m_specie_properties;
    std::map<QString, int> m_specie_name_to_id_mapper;
    DiceRoller m_dice_roller;
    SpecieToColorMapper m_plant_color_mapper;
};

#endif // PLANT_FACTORY_H
