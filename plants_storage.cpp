#include "plants_storage.h"
#include "plant.h"
#include <algorithm>
#include "boost/foreach.hpp"

PlantStorage::PlantStorage() : m_plants(), m_plant_count(0)
{

}

PlantStorage::~PlantStorage()
{
    clear();
}

void PlantStorage::add(Plant * p_plant)
{
    // Ensure specie exists in data collection
    auto it(m_plants.find(p_plant->m_specie_id));
    if(it == m_plants.end()) // Specie not yet inserted
    {
        m_plants.insert(std::pair<int, std::map<long, Plant*>>(p_plant->m_specie_id, std::map<long, Plant*>()));
    }

    // Insert the specie
    m_plants[p_plant->m_specie_id].insert(std::pair<long, Plant*>(p_plant->m_unique_id, p_plant));

    m_plant_count++;
}

void PlantStorage::remove(Plant * p_plant)
{
    if(contains(p_plant))
    {
        m_plants[p_plant->m_specie_id].erase(p_plant->m_unique_id);
        delete p_plant;
        m_plant_count--;
    }
}

bool PlantStorage::contains(Plant * p_plant) const
{
    auto it(m_plants.find(p_plant->m_specie_id));
    return (it != m_plants.end() && it->second.find(p_plant->m_unique_id) != it->second.end());
}

std::vector<Plant*> PlantStorage::getPlants()
{
    std::vector<Plant*> all_plants;

    for(auto specie(m_plants.begin()); specie!= m_plants.end(); specie++)
    {
        auto specie_plants(specie->second);
        for(auto plant(specie_plants.begin()); plant != specie_plants.end(); plant++)
        {
            all_plants.push_back(plant->second);
        }
    }
    return all_plants;
}

std::vector<Plant*> PlantStorage::getSortedPlants(SortingCriteria p_sorting_criteria)
{
    std::vector<Plant*> plants(getPlants());

    switch(p_sorting_criteria)
    {
    case SortingCriteria::Strength:
        std::sort(plants.begin(), plants.end(), [](Plant * lhs, Plant* rhs) {return lhs->getHeight() < rhs->getHeight();});
        break;
    case SortingCriteria::Height:
        std::sort(plants.begin(), plants.end(), [](Plant * lhs, Plant* rhs) {return lhs->getVigor() < rhs->getVigor();});
        break;
    }
    return plants;
}

void PlantStorage::clear()
{
    for(Plant * p : getPlants())
        delete p;

    m_plants.clear();
}

const PlantStorageStructure& PlantStorage::getSpecies()
{
    return m_plants;
}

int PlantStorage::getPlantCount()
{
    return m_plant_count;
}
