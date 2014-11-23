#ifndef PLANT_FACTORY_H
#define PLANT_FACTORY_H

#include <map>
#include "growth_manager.h"
#include <string>
#include <QColor>
#include "dice_roller.h"
#include <memory>
#include "helper.h"

class Plant;

enum Specie {
    OAK_TREE,
    OLIVE_TREE,
    PEANUT_TREE,
    BANANA_TREE
};

struct SpecieProperties {
    std::shared_ptr<AgeingProperties> ageing_properties;
    std::shared_ptr<GrowthProperties> growth_properties;
    std::shared_ptr<IlluminationProperties> illumination_properties;

    std::string name;
    QColor color;

    SpecieProperties(std::string p_name, QColor p_color, std::shared_ptr<AgeingProperties> p_ageing_properties,
                         std::shared_ptr<GrowthProperties> p_growth_properties, std::shared_ptr<IlluminationProperties> p_illumination_properties) :
        name(p_name), color(p_color), ageing_properties(p_ageing_properties), growth_properties(p_growth_properties),
        illumination_properties(p_illumination_properties){}
};

typedef std::map<Specie, QColor> PlantColorMap;

class PlantFactory {
public:
    PlantFactory();
    ~PlantFactory();
    Plant* generate(Specie type, Coordinate p_center_coord);
    PlantColorMap getSpeciesColorMapping() { return m_plant_to_color_map; }

private:
    void populate_db();
    void init_plant_properties();
    std::map<Specie, SpecieProperties> m_property_db;
    DiceRoller m_dice_roller;

    // NOTE: ALL PROBABILITIES IN PER THOUSAND
    /************
     * OAK TREE *
     ************/
    std::shared_ptr<GrowthProperties> oak_tree_growth_properties;
    std::shared_ptr<AgeingProperties> oak_tree_ageing_properties;
    std::shared_ptr<IlluminationProperties> oak_tree_illumination_properties;

    /**************
     * OLIVE TREE *
     **************/
    std::shared_ptr<GrowthProperties> olive_tree_growth_properties;
    std::shared_ptr<AgeingProperties> olive_tree_ageing_properties;
    std::shared_ptr<IlluminationProperties> olive_tree_illumination_properties;

    /***************
     * PEANUT TREE *
     ***************/
    std::shared_ptr<GrowthProperties> peanut_tree_growth_properties;
    std::shared_ptr<AgeingProperties> peanut_tree_ageing_properties;
    std::shared_ptr<IlluminationProperties> peanut_tree_illumination_properties;

    /***************
     * BANANA TREE *
     ***************/
    std::shared_ptr<GrowthProperties> banana_tree_growth_properties;
    std::shared_ptr<AgeingProperties> banana_tree_ageing_properties;
    std::shared_ptr<IlluminationProperties> banana_tree_illumination_properties;

    PlantColorMap m_plant_to_color_map;
};

#endif // PLANT_FACTORY_H
