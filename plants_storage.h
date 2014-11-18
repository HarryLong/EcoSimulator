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
    void addIntersectingPlantIds(int p_plant_id, const std::unordered_set<int> * p_intersecting_plant_ids);
    void removeIntersectingPlantId(int p_plant_id, int p_intersecting_id_to_remove);

    std::vector<Plant*> getSortedPlants();

private:
    std::map<long, Plant*> mapped_data;

    // For optimization purposes --> A sorted list of plants is required more often than a plant is removed
    std::vector<Plant*> raw_data;
};

#endif //PLANT_STORAGE_H
