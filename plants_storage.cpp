#include "plants_storage.h"
#include "plant.h"
#include <algorithm>
#include "boost/foreach.hpp"

PlantStorage::PlantStorage() : m_mapped_data(), m_specie_count()
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

    auto it(m_specie_count.find(p_plant->m_specie_id));
    if(it == m_specie_count.end())
        m_specie_count.insert(std::pair<int,int>(p_plant->m_specie_id,1));
    else
        it->second++;
}

void PlantStorage::remove(int p_plant_id)
{
    if(contains(p_plant_id))
    {
        Plant * p  = m_mapped_data[p_plant_id];
        m_raw_data.erase(std::find(m_raw_data.begin(), m_raw_data.end(), p)); // It will always be in here
        m_mapped_data.erase(p->m_unique_id);

        auto it(m_specie_count.find(p->m_specie_id));
        if(--it->second == 0)
            m_specie_count.erase(it);

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
    m_specie_count.clear();
}

const std::map<int,int>& PlantStorage::getSpecies()
{
    return m_specie_count;
}

int PlantStorage::getPlantCount()
{
    return m_raw_data.size();
}

