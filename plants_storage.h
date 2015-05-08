#ifndef PLANT_STORAGE_H
#define PLANT_STORAGE_H

#include <vector>
#include <map>
#include <unordered_set>
#include "spatial_hashmap.h"
#include <mutex>
#include "analyzer.h"

class Plant;

typedef std::map<int, std::map<long, Plant*> > PlantStorageStructure;
typedef std::pair<int,int> Position;

enum SortingCriteria{
    Strength,
    Height
};

struct LocationCell{
    std::map<int, std::map<Position, Plant*> > species;
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
    const PlantStorageStructure& getPlantsBySpecies();
    void generateSnapshot();
    void generateStatisticalSnapshot(std::pair<int,int> humidity, std::pair<int,int> illumination, std::pair<int,int> temperature, int elapsed_months);
    bool isPlantAtLocation(QPoint p_location);
    std::vector<Plant*> getOnePlantPerCell(int p_specie_id);
    const std::set<int> getSpecieIds();

    void lock();
    void unlock();

    typedef SpatialHashMap<LocationCell> PlantSpatialHashMap;

private:
    PlantStorageStructure m_plants;
    PlantSpatialHashMap m_location_plant_accessor;
    mutable std::mutex m_storage_accessor_mutex;
    int m_plant_count;

    AnalysisConfiguration m_statistical_analyzer_config;
};

#endif //PLANT_STORAGE_H
