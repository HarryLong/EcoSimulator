#ifndef PLANT_FACTORY_H
#define PLANT_FACTORY_H

#include <map>
#include "age_constrainer.h"
#include "growth_manager.h"
#include "illumination_constrainer.h"
#include <string>
#include <QRgb>
#include <auto_ptr.h>
#include "dice_roller.h"

class Plant;

enum Specie {
    OAK_TREE,
    OLIVE_TREE,
    PEANUT_TREE,
    BANANA_TREE
};

struct AggregatedProperties {
    const AgeingProperties * ageing_properties;
    const GrowthProperties * growth_properties;
    const IlluminationProperties * illumination_properties;

    std::string name;
    QRgb color;

    AggregatedProperties(std::string p_name, QRgb p_color, const AgeingProperties * p_ageing_properties,
                         const GrowthProperties * p_growth_properties, const IlluminationProperties * p_illumination_properties) :
        name(p_name), color(p_color), ageing_properties(p_ageing_properties), growth_properties(p_growth_properties),
        illumination_properties(p_illumination_properties){}
};

class PlantFactory {
public:
    PlantFactory();
    ~PlantFactory();
    Plant* generate(Specie type, Coordinate p_center_coord);

private:
    void populate_db();
    std::map<Specie, AggregatedProperties> m_property_db;
    DiceRoller m_dice_roller;

    // NOTE: ALL PROBABILITIES IN PER THOUSAND
    /************
     * OAK TREE *
     ************/
    const GrowthProperties* oak_tree_growth_properties = new GrowthProperties(
                                                                                .8f, // cm per month
                                                                                .5f  // Width is half the height
                                                                                );

    const AgeingProperties* oak_tree_ageing_properties = new AgeingProperties(
                                                                            100,   // probability of death at birth
                                                                            10, // probability of death at prime
                                                                            600,   // probability of death at max
                                                                            240,   // average prime age
                                                                            1200   // average max age
                                                                            );

    const IlluminationProperties* oak_tree_illumination_properties = new IlluminationProperties(980);



    /**************
     * OLIVE TREE *
     **************/
    const GrowthProperties* olive_tree_growth_properties = new GrowthProperties(
                                                                                2.2f, // cm per month
                                                                                .2f  // Width is half the height
                                                                                );

    const AgeingProperties* olive_tree_ageing_properties = new AgeingProperties(
                                                                            250,   // probability of death at birth
                                                                            75, // probability of death at prime
                                                                            850,   // probability of death at max
                                                                            60,   // average prime age
                                                                            180   // average max age
                                                                            );

    const IlluminationProperties* olive_tree_illumination_properties = new IlluminationProperties(950);

    /***************
     * PEANUT TREE *
     ***************/
    const GrowthProperties* peanut_tree_growth_properties = new GrowthProperties(
                                                                                1.15f, // cm per month
                                                                                .4f  // Width is half the height
                                                                                );

    const AgeingProperties* peanut_tree_ageing_properties = new AgeingProperties(
                                                                            30,   // probability of death at birth
                                                                            5, // probability of death at prime
                                                                            700,   // probability of death at max
                                                                            360,   // average prime age
                                                                            600   // average max age
                                                                            );

    const IlluminationProperties* peanut_tree_illumination_properties = new IlluminationProperties(960);

    /***************
     * BANANA TREE *
     ***************/
    const GrowthProperties* banana_tree_growth_properties = new GrowthProperties(
                                                                                2.1f, // cm per month
                                                                                .3f  // Width is half the height
                                                                                );

    const AgeingProperties* banana_tree_ageing_properties = new AgeingProperties(
                                                                            120,   // probability of death at birth
                                                                            20, // probability of death at prime
                                                                            700,   // probability of death at max
                                                                            480,   // average prime age
                                                                            720   // average max age
                                                                            );

    const IlluminationProperties* banana_tree_illumination_properties = new IlluminationProperties(950);

};

#endif // PLANT_FACTORY_H
