#include "plants_storage.h"
#include "plant.h"
#include <algorithm>
#include "boost/foreach.hpp"

PlantStorage::PlantStorage() : m_mapped_data()
{

}

PlantStorage::~PlantStorage()
{
    clear();
}

void PlantStorage::add(Plant * p_plant)
{
    m_mapped_data.insert(std::pair<long, Plant*>(p_plant->m_unique_id, p_plant));
    m_raw_data.push_back(p_plant);
}

void PlantStorage::remove(int p_plant_id)
{
    if(contains(p_plant_id))
    {
        Plant * p  = m_mapped_data[p_plant_id];
        m_raw_data.erase(std::find(m_raw_data.begin(), m_raw_data.end(), p)); // It will always be in here
        m_mapped_data.erase(p->m_unique_id);
        delete p;
    }
}

const Plant * PlantStorage::get(int p_plant_id) const
{
    auto it ( m_mapped_data.find(p_plant_id) );
    if( it  != m_mapped_data.end())
        return it->second;

    return NULL;
}

bool PlantStorage::contains(int p_plant_id) const
{
    return (m_mapped_data.find(p_plant_id) != m_mapped_data.end());
}

std::vector<Plant*> PlantStorage::getSortedPlants()
{
    std::sort(m_raw_data.begin(), m_raw_data.end(), [](Plant * lhs, Plant* rhs) {return lhs->getHeight() < rhs->getHeight();});

    return m_raw_data;
}

void PlantStorage::clear()
{
    for(auto it (m_mapped_data.begin()); it != m_mapped_data.end(); it++)
        delete it->second;

    m_mapped_data.clear();
    m_raw_data.clear();
}
