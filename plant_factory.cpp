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
}

static long unique_id = 1;
Plant* PlantFactory::generate(Specie p_specie, Coordinate p_center_coord)
{
    auto it = m_property_db.find(p_specie);

    if(it != m_property_db.end())
    {
        SpecieProperties specie_properties = it->second;
        return new Plant(specie_properties.name, specie_properties.color, p_center_coord, unique_id++, m_dice_roller.generate(),
                         specie_properties.growth_properties,
                         specie_properties.ageing_properties,
                         specie_properties.illumination_properties);
    }

    return NULL;
}

void PlantFactory::populate_db()
{
    /************
     * OAK TREE *
     ************/
    m_property_db.insert(std::pair<Specie, SpecieProperties>(OAK_TREE,
                                                                 SpecieProperties("Oak tree", m_plant_to_color_map[OAK_TREE],
                                                                                      oak_tree_ageing_properties, oak_tree_growth_properties,
                                                                                      oak_tree_illumination_properties)));
    /**************
     * OLIVE TREE *
     **************/
    m_property_db.insert(std::pair<Specie, SpecieProperties>(OLIVE_TREE,
                                                                 SpecieProperties("Olive Tree", m_plant_to_color_map[OLIVE_TREE],
                                                                                      olive_tree_ageing_properties, olive_tree_growth_properties,
                                                                                      olive_tree_illumination_properties)));
    /***************
     * PEANUT TREE *
     ***************/
    m_property_db.insert(std::pair<Specie, SpecieProperties>(PEANUT_TREE,
                                                                 SpecieProperties("Peanut Tree", m_plant_to_color_map[PEANUT_TREE],
                                                                                      peanut_tree_ageing_properties, peanut_tree_growth_properties,
                                                                                      peanut_tree_illumination_properties)));
    /***************
     * BANANA TREE *
     ***************/
    m_property_db.insert(std::pair<Specie, SpecieProperties>(BANANA_TREE,
                                                                 SpecieProperties("Banana Tree", m_plant_to_color_map[BANANA_TREE],
                                                                                      banana_tree_ageing_properties, banana_tree_growth_properties,
                                                                                      banana_tree_illumination_properties)));
}

void PlantFactory::init_plant_properties()
{
    m_plant_to_color_map[OAK_TREE] = QColor(Qt::GlobalColor::red);
    m_plant_to_color_map[OLIVE_TREE] = QColor(Qt::GlobalColor::green);
    m_plant_to_color_map[PEANUT_TREE] = QColor(Qt::GlobalColor::blue);
    m_plant_to_color_map[BANANA_TREE] = QColor(Qt::GlobalColor::yellow);

    oak_tree_growth_properties = std::shared_ptr<GrowthProperties>(new GrowthProperties(.8f, // cm per month
                                                                                        .5f  // Width is half the height
                                                                                        ));

    oak_tree_ageing_properties = std::shared_ptr<AgeingProperties>(new AgeingProperties(10.f/100, // probability of death at birth
                                                                                        60.f/100, // probability of death at upper
                                                                                        240, // age of prime
                                                                                        1200)); // upper limit age

    oak_tree_illumination_properties = std::shared_ptr<IlluminationProperties>(new IlluminationProperties(10.f/100, // Start of negative impact
                                                                                                          60.f/.100, // Max shade
                                                                                                          85.f/100)); // Probability of death at max shade



    /**************
     * OLIVE TREE *
     **************/
    olive_tree_growth_properties = std::shared_ptr<GrowthProperties>(new GrowthProperties(2.2f, // cm per month
                                                                                          .2f  // Width is half the height
                                                                                          ));

    olive_tree_ageing_properties = std::shared_ptr<AgeingProperties>(new AgeingProperties(25.f/100, // probability of death at birth
                                                                                          85.f/100, // probability of death at upper
                                                                                          60, // age of prime
                                                                                          180)); // upper limit age

    olive_tree_illumination_properties = std::shared_ptr<IlluminationProperties>(new IlluminationProperties(20.f/100, // Start of negative impact
                                                                                                          60.f/.100, // Max shade
                                                                                                          85.f/100)); // Probability of death at max shade


    /***************
     * PEANUT TREE *
     ***************/
    peanut_tree_growth_properties = std::shared_ptr<GrowthProperties>(new GrowthProperties(1.15f, // cm per month
                                                                                          .4f  // Width is half the height
                                                                                          ));

    peanut_tree_ageing_properties = std::shared_ptr<AgeingProperties>(new AgeingProperties(3.f/100, // probability of death at birth
                                                                                           70.f/100, // probability of death at upper
                                                                                           360, // age of prime
                                                                                           600)); // upper limit age

    peanut_tree_illumination_properties = std::shared_ptr<IlluminationProperties>(new IlluminationProperties(20.f/100, // Start of negative impact
                                                                                                          60.f/.100, // Max shade
                                                                                                          85.f/100)); // Probability of death at max shade


    /***************
     * BANANA TREE *
     ***************/
    banana_tree_growth_properties = std::shared_ptr<GrowthProperties>(new GrowthProperties(2.1f, // cm per month
                                                                                          .3f  // Width is half the height
                                                                                          ));

    banana_tree_ageing_properties = std::shared_ptr<AgeingProperties>(new AgeingProperties(12.f/100, // probability of death at birth
                                                                                           70.f/100, // probability of death at upper
                                                                                           480, // age of prime
                                                                                           720)); // upper limit age

    banana_tree_illumination_properties = std::shared_ptr<IlluminationProperties>(new IlluminationProperties(20.f/100, // Start of negative impact
                                                                                                          60.f/.100, // Max shade
                                                                                                          85.f/100)); // Probability of death at max shade
}
