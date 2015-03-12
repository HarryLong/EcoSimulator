#ifndef PLANT_STORAGE_H
#define PLANT_STORAGE_H

#include <vector>
#include <map>
#include <unordered_set>

class Plant;

typedef std::map<int, std::map<long, Plant*>> PlantStorageStructure;

enum SortingCriteria{
    Strength,
    Height
};

class PlantStorage{

public:
    PlantStorage();
    ~PlantStorage();
    void add(Plant * p_plant);
    void remove(Plant * p_plant);
    bool contains(Plant * p_plant) const;
    void clear();
    int getPlantCount();
    std::vector<Plant*> getPlants();
    std::vector<Plant*> getSortedPlants(SortingCriteria p_sorting_criteria);
    const PlantStorageStructure& getSpecies();

private:
    PlantStorageStructure m_plants;
    int m_plant_count;

//    std::map<long, Plant*> m_plant_id_mapper;
//    std::map<int,std::vector<long> > m_specie_id_mapper;

    // For optimization purposes --> A sorted list of plants is required more often than a plant is removed
//    std::vector<Plant*> m_raw_data;
};

#endif //PLANT_STORAGE_H
