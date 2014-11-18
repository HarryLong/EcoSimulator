#include "plants_storage.h"
#include "plant.h"
#include <algorithm>
#include "boost/foreach.hpp"

PlantStorage::PlantStorage() : mapped_data()
{

}

PlantStorage::~PlantStorage()
{
    for(auto it (mapped_data.begin()); it != mapped_data.end(); it++)
        delete it->second;
}

void PlantStorage::add(Plant * p_plant)
{
    mapped_data.insert(std::pair<long, Plant*>(p_plant->m_unique_id, p_plant));
    raw_data.push_back(p_plant);
}

void PlantStorage::remove(int p_plant_id)
{
    if(contains(p_plant_id))
    {
        Plant * p  = mapped_data[p_plant_id];

        raw_data.erase(std::find(raw_data.begin(), raw_data.end(), p)); // It will always be in here
        mapped_data.erase(p->m_unique_id);
        delete p;
    }
}

const Plant * PlantStorage::get(int p_plant_id) const
{
    auto it ( mapped_data.find(p_plant_id) );
    if( it  != mapped_data.end())
        return it->second;

    return NULL;
}

bool PlantStorage::contains(int p_plant_id) const
{
    return (mapped_data.find(p_plant_id) != mapped_data.end());
}

std::vector<Plant*> PlantStorage::getSortedPlants()
{
    std::sort(raw_data.begin(), raw_data.end(), [](Plant * lhs, Plant* rhs) {return lhs->getHeight() < rhs->getHeight();});

    return raw_data;
}

void PlantStorage::addIntersectingPlantIds(int p_plant_id, const std::unordered_set<int> * p_intersecting_plant_ids)
{
    Plant * p  = mapped_data[p_plant_id];

    for(int id : *p_intersecting_plant_ids)
    {
        p->addIntersectingPlantId(id);
    }
}

void PlantStorage::removeIntersectingPlantId(int p_plant_id, int p_intersecting_id_to_remove)
{
    mapped_data[p_plant_id]->removeIntersectingPlantId(p_intersecting_id_to_remove);
}
