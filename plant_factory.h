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

class Plant;

static const QString specie_name_oak_tree = "Oak Tree";
static const QString specie_name_olive_tree = "Olive Tree";
static const QString specie_name_peanut_tree = "Peanut Tree";
static const QString specie_name_banana_tree = "Banana Tree";

struct SpecieProperties {
    std::shared_ptr<AgeingProperties> ageing_properties;
    std::shared_ptr<GrowthProperties> growth_properties;
    std::shared_ptr<IlluminationProperties> illumination_properties;
    std::shared_ptr<SoilHumidityProperties> soil_humidity_properties;

    QString name;
    QColor color;

    SpecieProperties(QString p_name, QColor p_color, std::shared_ptr<AgeingProperties> p_ageing_properties,
                         std::shared_ptr<GrowthProperties> p_growth_properties, std::shared_ptr<IlluminationProperties> p_illumination_properties,
                         std::shared_ptr<SoilHumidityProperties> p_soil_humidity_properties) :
        name(p_name), color(p_color),
        ageing_properties(p_ageing_properties),
        growth_properties(p_growth_properties),
        illumination_properties(p_illumination_properties),
        soil_humidity_properties(p_soil_humidity_properties){}
};

typedef std::map<QString, QColor> PlantColorMap;

class PlantFactory {
public:
    PlantFactory();
    ~PlantFactory();
    Plant* generate(QString p_specie_name, QPoint p_center_coord);
    PlantColorMap getSpeciesColorMapping() { return m_plant_to_color_map; }
    std::vector<QString> getAllSpecieNames();


private:
    void populate_db();
    void init_plant_properties();
    std::map<QString, SpecieProperties> m_plant_db;
    DiceRoller m_dice_roller;

    /************
     * OAK TREE *
     ************/
    std::shared_ptr<GrowthProperties> oak_tree_growth_properties;
    std::shared_ptr<AgeingProperties> oak_tree_ageing_properties;
    std::shared_ptr<IlluminationProperties> oak_tree_illumination_properties;
    std::shared_ptr<SoilHumidityProperties> oak_tree_soil_humidity_properties;

    /**************
     * OLIVE TREE *
     **************/
    std::shared_ptr<GrowthProperties> olive_tree_growth_properties;
    std::shared_ptr<AgeingProperties> olive_tree_ageing_properties;
    std::shared_ptr<IlluminationProperties> olive_tree_illumination_properties;
    std::shared_ptr<SoilHumidityProperties> olive_tree_soil_humidity_properties;

    /***************
     * PEANUT TREE *
     ***************/
    std::shared_ptr<GrowthProperties> peanut_tree_growth_properties;
    std::shared_ptr<AgeingProperties> peanut_tree_ageing_properties;
    std::shared_ptr<IlluminationProperties> peanut_tree_illumination_properties;
    std::shared_ptr<SoilHumidityProperties> peanut_tree_soil_humidity_properties;

    /***************
     * BANANA TREE *
     ***************/
    std::shared_ptr<GrowthProperties> banana_tree_growth_properties;
    std::shared_ptr<AgeingProperties> banana_tree_ageing_properties;
    std::shared_ptr<IlluminationProperties> banana_tree_illumination_properties;
    std::shared_ptr<SoilHumidityProperties> banana_tree_soil_humidity_properties;

    PlantColorMap m_plant_to_color_map;

    PlantDB m_real_plant_db;
};

#endif // PLANT_FACTORY_H
