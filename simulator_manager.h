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
#include "environment_manager.h"

#include "debuger.h"

#include "plant.h"

enum SimulationState {
    Running,
    Paused,
    Stopped
};

class SimulatorManager : public QObject, public TimeListener
{
    Q_OBJECT
public:
    SimulatorManager();
    ~SimulatorManager();
    int getElapsedMonths() { return m_elapsed_months; }

    void setMonthlyTriggerFrequency(int p_frequency);

    virtual void newMonth();
//    SpatialHashMap getSpatialHashMap() { return m_plant_spatial_hashmap; }
    SimulationState getState() { return m_state; }

    PlantRenderDataContainer getPlantRenderingData();
    IlluminationSpatialHashMap getIlluminationRenderingData();

public slots:
    void start();
    void pause();
    void resume();
    void stop();

signals:
    void update(void);
    void newPlant(QString name,QColor color);
    void removedPlant(QString name, QString cause_of_death);

private:
    void add_plant(Plant * p_plant);
    void remove_plant(int p_plant_id);

    EnvironmentManager m_environment_mgr;

    TimeManager m_time_keeper;

    PlantRenderDataContainer m_plant_rendering_data;

    PlantStorage m_plant_storage;
    PlantFactory m_plant_factory;

    QString plant_status_to_string(PlantStatus status);

    int m_elapsed_months;
    std::atomic<bool> m_stopping;
    SimulationState m_state;

    // TEMPORARY
    void generate_random_plants();
    void add(Specie specie, int count);
    Coordinate generate_random_position();
};

#endif //SIMULATOR_MANAGER_H
