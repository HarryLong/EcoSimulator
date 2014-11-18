#include "plant_factory.h"
#include <QColor>
#include "plant.h"

PlantFactory::PlantFactory() : m_property_db(), m_dice_roller(0,1000)
{
    init_plant_properties();
    populate_db();
}

PlantFactory::~PlantFactory()
{
//    delete oak_tree_growth_properties;
//    delete oak_tree_ageing_properties;

//    delete olive_tree_growth_properties;
//    delete olive_tree_ageing_properties;

//    delete peanut_tree_growth_properties;
//    delete peanut_tree_ageing_properties;

//    delete banana_tree_growth_properties;
//    delete banana_tree_ageing_properties;
}

static long unique_id = 1;
Plant* PlantFactory::generate(Specie p_specie, Coordinate p_center_coord)
{
    auto it = m_property_db.find(p_specie);

    if(it != m_property_db.end())
    {
        AggregatedProperties aggregated_properties = it->second;
        return new Plant(aggregated_properties.name, aggregated_properties.color, p_center_coord, unique_id++, m_dice_roller.generate(),
                         new GrowthManager(aggregated_properties.growth_properties), new AgeConstrainer(aggregated_properties.ageing_properties),
                         new IlluminationConstrainer(aggregated_properties.illumination_properties));
    }

    return NULL;
}

void PlantFactory::populate_db()
{
    /************
     * OAK TREE *
     ************/
    m_property_db.insert(std::pair<Specie, AggregatedProperties>(OAK_TREE,
                                                                 AggregatedProperties("Oak tree", m_plant_to_color_map[OAK_TREE],
                                                                                      oak_tree_ageing_properties, oak_tree_growth_properties,
                                                                                      oak_tree_illumination_properties)));
    /**************
     * OLIVE TREE *
     **************/
    m_property_db.insert(std::pair<Specie, AggregatedProperties>(OLIVE_TREE,
                                                                 AggregatedProperties("Olive Tree", m_plant_to_color_map[OLIVE_TREE],
                                                                                      olive_tree_ageing_properties, olive_tree_growth_properties,
                                                                                      olive_tree_illumination_properties)));
    /***************
     * PEANUT TREE *
     ***************/
    m_property_db.insert(std::pair<Specie, AggregatedProperties>(PEANUT_TREE,
                                                                 AggregatedProperties("Peanut Tree", m_plant_to_color_map[PEANUT_TREE],
                                                                                      peanut_tree_ageing_properties, peanut_tree_growth_properties,
                                                                                      peanut_tree_illumination_properties)));
    /***************
     * BANANA TREE *
     ***************/
    m_property_db.insert(std::pair<Specie, AggregatedProperties>(BANANA_TREE,
                                                                 AggregatedProperties("Banana Tree", m_plant_to_color_map[BANANA_TREE],
                                                                                      banana_tree_ageing_properties, banana_tree_growth_properties,
                                                                                      banana_tree_illumination_properties)));
}

void PlantFactory::init_plant_properties()
{
    m_plant_to_color_map[OAK_TREE] = QColor(Qt::GlobalColor::red);
    m_plant_to_color_map[OLIVE_TREE] = QColor(Qt::GlobalColor::green);
    m_plant_to_color_map[PEANUT_TREE] = QColor(Qt::GlobalColor::blue);
    m_plant_to_color_map[BANANA_TREE] = QColor(Qt::GlobalColor::yellow);

    oak_tree_growth_properties = std::shared_ptr<GrowthProperties>(new GrowthProperties(
                                                                                        .8f, // cm per month
                                                                                        .5f  // Width is half the height
                                                                                        ));

    oak_tree_ageing_properties = std::shared_ptr<AgeingProperties>(new AgeingProperties(
                                                                                        100,   // probability of death at birth
                                                                                        10, // probability of death at prime
                                                                                        600,   // probability of death at max
                                                                                        240,   // average prime age
                                                                                        1200   // average max age
                                                                                        ));

    oak_tree_illumination_properties = std::shared_ptr<IlluminationProperties>(new IlluminationProperties(200));



    /**************
     * OLIVE TREE *
     **************/
    olive_tree_growth_properties = std::shared_ptr<GrowthProperties> (new GrowthProperties(
                                                                                2.2f, // cm per month
                                                                                .2f  // Width is half the height
                                                                                ));

    olive_tree_ageing_properties = std::shared_ptr<AgeingProperties>(new AgeingProperties(
                                                                            250,   // probability of death at birth
                                                                            75, // probability of death at prime
                                                                            850,   // probability of death at max
                                                                            60,   // average prime age
                                                                            180   // average max age
                                                                            ));

    olive_tree_illumination_properties = std::shared_ptr<IlluminationProperties>(new IlluminationProperties(80));

    /***************
     * PEANUT TREE *
     ***************/
    peanut_tree_growth_properties = std::shared_ptr<GrowthProperties> (new GrowthProperties(
                                                                                1.15f, // cm per month
                                                                                .4f  // Width is half the height
                                                                                ));

    peanut_tree_ageing_properties = std::shared_ptr<AgeingProperties>(new AgeingProperties(
                                                                            30,   // probability of death at birth
                                                                            5, // probability of death at prime
                                                                            700,   // probability of death at max
                                                                            360,   // average prime age
                                                                            600   // average max age
                                                                            ));

    peanut_tree_illumination_properties = std::shared_ptr<IlluminationProperties>(new IlluminationProperties(200));

    /***************
     * BANANA TREE *
     ***************/
    banana_tree_growth_properties = std::shared_ptr<GrowthProperties>(new GrowthProperties(
                                                                                2.1f, // cm per month
                                                                                .3f  // Width is half the height
                                                                                ));

    banana_tree_ageing_properties = std::shared_ptr<AgeingProperties>(new AgeingProperties(
                                                                            120,   // probability of death at birth
                                                                            20, // probability of death at prime
                                                                            700,   // probability of death at max
                                                                            480,   // average prime age
                                                                            720   // average max age
                                                                            ));

    banana_tree_illumination_properties = std::shared_ptr<IlluminationProperties>(new IlluminationProperties(100));
}
