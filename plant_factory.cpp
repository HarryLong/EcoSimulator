#include "plant_factory.h"
#include <QColor>
#include "plant.h"

PlantFactory::PlantFactory() : m_property_db(), m_dice_roller(0,1000)
{
    populate_db();
}

PlantFactory::~PlantFactory()
{
    delete oak_tree_growth_properties;
    delete oak_tree_ageing_properties;
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
                                                                 AggregatedProperties("Oak tree", QColor(Qt::GlobalColor::red).rgb(),
                                                                                      oak_tree_ageing_properties, oak_tree_growth_properties,
                                                                                      oak_tree_illumination_properties)));
    /**************
     * OLIVE TREE *
     **************/
    m_property_db.insert(std::pair<Specie, AggregatedProperties>(OLIVE_TREE,
                                                                 AggregatedProperties("Olive Tree", QColor(Qt::GlobalColor::green).rgb(),
                                                                                      olive_tree_ageing_properties, olive_tree_growth_properties,
                                                                                      olive_tree_illumination_properties)));
    /***************
     * PEANUT TREE *
     ***************/
    m_property_db.insert(std::pair<Specie, AggregatedProperties>(PEANUT_TREE,
                                                                 AggregatedProperties("Peanut Tree", QColor(Qt::GlobalColor::blue).rgb(),
                                                                                      peanut_tree_ageing_properties, peanut_tree_growth_properties,
                                                                                      peanut_tree_illumination_properties)));
    /***************
     * BANANA TREE *
     ***************/
    m_property_db.insert(std::pair<Specie, AggregatedProperties>(BANANA_TREE,
                                                                 AggregatedProperties("Banana Tree", QColor(Qt::GlobalColor::yellow).rgb(),
                                                                                      banana_tree_ageing_properties, banana_tree_growth_properties,
                                                                                      banana_tree_illumination_properties)));
}
