#ifndef PLANT_FACTORY_H
#define PLANT_FACTORY_H

#include <map>
#include <string>
#include <QColor>
#include <memory>
#include <stack>

#include "../../math/dice_roller.h"
#include "plantDB/plant_db.h"
#include "plant.h"
#include <QPoint>

class PlantFactory {
public:
    PlantFactory(int area_width, int area_height);
    ~PlantFactory();
    Plant generate(QString p_specie_name, QPoint p_center_coord);
    Plant generate(QString p_specie_name);
    Plant generate(int p_specie_id, QPoint p_center_coord);
    Plant generate(int p_specie_id);
    std::vector<QString> getAllSpecieNames();
    const SpecieProperties & getSpecieProperties(int p_specie_id);

private:
    int get_specie_id(const QString & name);
    QPoint generate_random_position();

    int m_area_width, m_area_height;
    PlantDB::SpeciePropertiesHolder m_specie_properties;
    std::map<QString, int> m_specie_name_to_id_mapper;
    DiceRoller m_dice_roller;

    static std::map<int, int> specie_id_to_color_index;
    static std::vector<QColor> get_specie_colors();
    static int color_index;
    const static std::vector<QColor> _COLORS;
};

#endif // PLANT_FACTORY_H
