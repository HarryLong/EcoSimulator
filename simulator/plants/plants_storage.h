#ifndef PLANT_STORAGE_H
#define PLANT_STORAGE_H

#include <vector>
#include <map>
#include <unordered_set>
#include <mutex>

#include <radialDistribution/analyser/analysis_configuration.h>
#include "SpatialHashmap/spatial_hashmap.h"
#include <QPoint>
#include <QHash>

class Plant;

typedef std::map<int, std::map<long, Plant> > PlantStorageStructure;

enum SortingCriteria{
    Strength,
    Height
};

class LocationCell{
public:
    LocationCell() : species() {}
    std::map<int, std::map<QPoint, Plant> > species;
};

class PlantStorage{
public:
    PlantStorage(int area_width, int area_height);
    ~PlantStorage();
    void add(Plant p_plant);
    void remove(Plant p_plant);
    bool contains(Plant p_plant) const;
    void clear();
    int getPlantCount();
    std::vector<Plant> getPlants();
    std::list<Plant> getSortedPlants(SortingCriteria p_sorting_criteria);
    const PlantStorageStructure& getPlantsBySpecies();
    void generateSnapshot();
    void generateStatisticalSnapshot(std::vector<int> humidities, std::vector<int> illuminations, std::vector<int> temperatures, int elapsed_months);
    bool isPlantAtLocation(QPoint p_location);
    std::vector<Plant> getOnePlantPerCell(int p_specie_id);
    const std::set<int> getSpecieIds();

    void lock();
    void unlock();

    typedef SpatialHashMap<LocationCell> PlantSpatialHashMap;

private:
    PlantStorageStructure m_plants;
    PlantSpatialHashMap m_location_plant_accessor;
    mutable std::mutex m_storage_accessor_mutex;
    int m_plant_count;
    int m_area_width, m_area_height;

//    AnalysisConfiguration m_statistical_analyzer_config;
};

#endif //PLANT_STORAGE_H
