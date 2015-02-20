#ifndef PLANT_STORAGE_H
#define PLANT_STORAGE_H

#include <vector>
#include <map>
#include <unordered_set>

class Plant;

class PlantStorage{

public:
    PlantStorage();
    ~PlantStorage();
    void add(Plant * p_plant);
    void remove(int p_plant_id);
    const Plant * get(int p_plant_id) const;
    bool contains(int p_plant_id) const;
    void clear();
    int getPlantCount();

    std::vector<Plant*> getSortedPlants();
    const std::map<int,int>& getSpecies();

private:
    std::map<long, Plant*> m_mapped_data;
    std::map<int,int> m_specie_count;

    // For optimization purposes --> A sorted list of plants is required more often than a plant is removed
    std::vector<Plant*> m_raw_data;
};

#endif //PLANT_STORAGE_H
