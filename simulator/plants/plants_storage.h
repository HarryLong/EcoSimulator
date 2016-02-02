#ifndef PLANT_STORAGE_H
#define PLANT_STORAGE_H

#include <vector>
#include <map>
#include <unordered_set>
#include <mutex>

#include "../../resources/environment_manager.h"
#include <radialDistribution/analyser/analysis_configuration.h>
#include "SpatialHashmap/spatial_hashmap.h"
#include <QPoint>
#include <QHash>
#include <vector>
#include <exception>

#include "plant.h"

enum SortingCriteria{
    Strength,
    Height
};

class LocationCell{
public:
    typedef std::map<int, std::unordered_map<QPoint, int> > SpecieLocationQueryablePlants;
    LocationCell() : species() {}
    SpecieLocationQueryablePlants species;
};
class CallbackListener;
class PlantStorage{
public:
    typedef SpatialHashMap<LocationCell> PlantSpatialHashMap;
    typedef std::unordered_map<int, Plant> BasePlantStorage;
    typedef std::unordered_map<int, std::unordered_set<int> > SpecieQueryablePlants;

    class InvalidPlantIDException : public std::exception
    {
    public:
        virtual const char* what() const noexcept
        {
            return "Requested invalid plant id!";
        }
    };

    PlantStorage(int area_width, int area_height);
    ~PlantStorage();
    void add(const Plant & p_plant, bool mutex_lock = true);
    void remove(const Plant & plant, bool mutex_lock = true);
    void clear(bool mutex_lock = true);
    int getPlantCount() const;
    std::vector<Plant> getPlants(bool mutex_lock = true) const;
    std::list<Plant> getSortedPlants(SortingCriteria p_sorting_criteria, bool mutex_lock = true) const;
    bool isPlantAtLocation(QPoint p_location, bool mutex_lock = true) const;
    std::set<int> getSpecieIds(bool mutex_lock = true) const;
    std::vector<Plant> getOnePlantPerCell(int p_specie_id, bool mutex_lock = true) const;
    bool containsSpecie(int specie_id, bool mutex_lock = true) const;

    SpecieQueryablePlants getPlantsBySpecies();
    void generateSnapshot(bool mutex_lock = true) const;
    void generateStatisticalSnapshot(float slope, std::vector<int> humidities, std::vector<int> illuminations, std::vector<int> temperatures, int elapsed_months,
                                     CallbackListener * work_completion_listener = nullptr, bool mutex_lock = true);
    void update(EnvironmentManager & environment_manager, std::vector<Plant> & surviving_plants, std::vector<Plant> & deceased_plants, bool mutex_lock = true);

private:
    Plant operator[](int plant_id) const;
    bool contains_plant(int plant_id, bool mutex_lock = true) const;
    void lock() const;
    void unlock() const;

    BasePlantStorage m_plants;
    SpecieQueryablePlants m_specie_id_queryable_plants;
    PlantSpatialHashMap m_location_queryable_plants;

    mutable std::mutex m_storage_accessor_mutex;
    int m_plant_count;
    int m_area_width, m_area_height;

    AnalysisConfiguration m_statistical_analyzer_config;
};

#endif //PLANT_STORAGE_H
