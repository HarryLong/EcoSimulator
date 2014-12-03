#include "plant_factory.h"
#include <QColor>
#include "plant.h"

PlantFactory::PlantFactory() : m_plant_db(), m_dice_roller(0,1000), m_real_plant_db()
{
    init_plant_properties();
    populate_db();
}

PlantFactory::~PlantFactory()
{
}

static long unique_id = 1;
Plant* PlantFactory::generate(QString p_specie_name, QPoint p_center_coord)
{
    auto it = m_plant_db.find(p_specie_name);

    if(it != m_plant_db.end())
    {
        SpecieProperties specie_properties = it->second;
        return new Plant(specie_properties.name, specie_properties.color, p_center_coord, unique_id++, m_dice_roller.generate(),
                         specie_properties.growth_properties,
                         specie_properties.ageing_properties,
                         specie_properties.illumination_properties,
                         specie_properties.soil_humidity_properties);
    }

    return NULL;
}

std::vector<QString> PlantFactory::getAllSpecieNames()
{
    std::vector<QString> ret;

    for(auto it (m_plant_db.begin()); it != m_plant_db.end(); it++)
        ret.push_back(it->first);

    return ret;
}

void PlantFactory::populate_db()
{
    /************
     * OAK TREE *
     ************/
    m_plant_db.insert(std::pair<QString, SpecieProperties>(specie_name_oak_tree,
                                                                 SpecieProperties(specie_name_oak_tree, m_plant_to_color_map[specie_name_oak_tree],
                                                                                      oak_tree_ageing_properties,
                                                                                      oak_tree_growth_properties,
                                                                                      oak_tree_illumination_properties,
                                                                                      oak_tree_soil_humidity_properties)));
    /**************
     * OLIVE TREE *
     **************/
    m_plant_db.insert(std::pair<QString, SpecieProperties>(specie_name_olive_tree,
                                                                 SpecieProperties(specie_name_olive_tree, m_plant_to_color_map[specie_name_olive_tree],
                                                                                      olive_tree_ageing_properties,
                                                                                      olive_tree_growth_properties,
                                                                                      olive_tree_illumination_properties,
                                                                                      olive_tree_soil_humidity_properties)));
    /***************
     * PEANUT TREE *
     ***************/
    m_plant_db.insert(std::pair<QString, SpecieProperties>(specie_name_peanut_tree,
                                                                 SpecieProperties(specie_name_peanut_tree, m_plant_to_color_map[specie_name_peanut_tree],
                                                                                      peanut_tree_ageing_properties,
                                                                                      peanut_tree_growth_properties,
                                                                                      peanut_tree_illumination_properties,
                                                                                      peanut_tree_soil_humidity_properties)));
    /***************
     * BANANA TREE *
     ***************/
    m_plant_db.insert(std::pair<QString, SpecieProperties>(specie_name_banana_tree,
                                                                 SpecieProperties(specie_name_banana_tree, m_plant_to_color_map[specie_name_banana_tree],
                                                                                      banana_tree_ageing_properties,
                                                                                      banana_tree_growth_properties,
                                                                                      banana_tree_illumination_properties,
                                                                                      banana_tree_soil_humidity_properties)));
}

void PlantFactory::init_plant_properties()
{
    m_plant_to_color_map[specie_name_oak_tree] = QColor(Qt::GlobalColor::red);
    m_plant_to_color_map[specie_name_olive_tree] = QColor(Qt::GlobalColor::green);
    m_plant_to_color_map[specie_name_peanut_tree] = QColor(Qt::GlobalColor::blue);
    m_plant_to_color_map[specie_name_banana_tree] = QColor(Qt::GlobalColor::yellow);

    oak_tree_growth_properties = std::shared_ptr<GrowthProperties>(new GrowthProperties(.8f, // cm per month
                                                                                        1.2f,
                                                                                        .5f  // Width is half the height
                                                                                        ));

    oak_tree_ageing_properties = std::shared_ptr<AgeingProperties>(new AgeingProperties(10, // probability of death at birth
                                                                                        60, // probability of death at upper
                                                                                        100, // age of prime start
                                                                                        1000, // age of prime end
                                                                                        1200)); // upper limit age

    oak_tree_illumination_properties = std::shared_ptr<IlluminationProperties>(new IlluminationProperties(10, // Start of negative impact
                                                                                                          90.f)); // Probability of death when completely shaded

    oak_tree_soil_humidity_properties = std::shared_ptr<SoilHumidityProperties>(new SoilHumidityProperties(30,50));



    /**************
     * OLIVE TREE *
     **************/
    olive_tree_growth_properties = std::shared_ptr<GrowthProperties>(new GrowthProperties(2.f, // cm per month
                                                                                          1.f,
                                                                                          .2f  // Width is half the height
                                                                                          ));

    olive_tree_ageing_properties = std::shared_ptr<AgeingProperties>(new AgeingProperties(25, // probability of death at birth
                                                                                          85, // probability of death at upper
                                                                                          20, // age of prime start
                                                                                          150, // age of prime end
                                                                                          180)); // upper limit age

    olive_tree_illumination_properties = std::shared_ptr<IlluminationProperties>(new IlluminationProperties(20, // Start of negative impact
                                                                                                          98)); // Probability of death when completely shaded

    olive_tree_soil_humidity_properties = std::shared_ptr<SoilHumidityProperties>(new SoilHumidityProperties(10,20));


    /***************
     * PEANUT TREE *
     ***************/
    peanut_tree_growth_properties = std::shared_ptr<GrowthProperties>(new GrowthProperties(1.1f, // cm per month
                                                                                           .2f,
                                                                                          .4f  // Width is half the height
                                                                                          ));

    peanut_tree_ageing_properties = std::shared_ptr<AgeingProperties>(new AgeingProperties(3, // probability of death at birth
                                                                                           70, // probability of death at upper
                                                                                           50, // age of prime start
                                                                                           500, // age of prime end
                                                                                           600)); // upper limit age

    peanut_tree_illumination_properties = std::shared_ptr<IlluminationProperties>(new IlluminationProperties(20, // Start of negative impact
                                                                                                          85)); // Probability of death at max shade

    peanut_tree_soil_humidity_properties = std::shared_ptr<SoilHumidityProperties>(new SoilHumidityProperties(20,50));

    /***************
     * BANANA TREE *
     ***************/
    banana_tree_growth_properties = std::shared_ptr<GrowthProperties>(new GrowthProperties(2.1f, // height growth (cm per month)
                                                                                           .5f,
                                                                                          .3f  // Width is half the height
                                                                                          ));

    banana_tree_ageing_properties = std::shared_ptr<AgeingProperties>(new AgeingProperties(12, // probability of death at birth
                                                                                           70, // probability of death at upper
                                                                                           100, // age of prime start
                                                                                           600, // age of prime end
                                                                                           720)); // upper limit age

    banana_tree_illumination_properties = std::shared_ptr<IlluminationProperties>(new IlluminationProperties(20, // Start of negative impact
                                                                                                          85)); // Probability of death at max shade

    banana_tree_soil_humidity_properties = std::shared_ptr<SoilHumidityProperties>(new SoilHumidityProperties(50,70));

}
