#ifndef SIMULATOR_MANAGER_H
#define SIMULATOR_MANAGER_H

#include "uchar.h"
#include "time_manager.h"
#include "renderer.h"
#include "plant_factory.h"
#include "plants_storage.h"
#include "helper.h"
#include "constants.h"

#include <QObject>
#include <vector>
#include <map>
#include <unordered_set>

class Plant;

class SimulatorManager : public QObject, public TimeListener
{
    Q_OBJECT
public:
    SimulatorManager();
    ~SimulatorManager();
    void start();
    void stop();
    virtual void trigger();
    void addPlant(Plant * p_plant);
    int getElapsedMonths() { return m_elapsed_months; }

    PlantRenderDataContainer getPlantData();
    PlantBoundingBoxContainer getBoundingBoxData();
    void setMonthlyTriggerFrequency(int p_frequency);

signals:
    void update();

private:
    TimeManager m_time_keeper;

    PlantBoundingBoxContainer m_world_plant_id_ref;
    PlantRenderDataContainer m_plant_rendering_data;
    std::map<long, int> m_drawn_radii;
    PlantStorage m_plant_storage;
    PlantFactory m_plant_factory;

    void remove_plant_data(Plant* p_plant);
    int m_elapsed_months;

    // TEMPORARY
    void generate_random_plants();
    Coordinate to_coord(int p_index);
    int to_index(Coordinate p_coord);
    int get_random_index();
};

#endif //SIMULATOR_MANAGER_H
