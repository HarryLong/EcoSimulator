#ifndef SIMULATOR_MANAGER_H
#define SIMULATOR_MANAGER_H

#include "../../utils/time_manager.h"
#include "../plants/plant_factory.h"
#include "../plants/plants_storage.h"
#include "../../resources/environment_manager.h"
#include "../plants/plant.h"
#include "simulation_configuration.h"

#ifdef GUI_MODE
#include "../../data_holders/plant_rendering_data_container.h"
#endif

#include <QObject>
#include <vector>
#include <map>
#include <unordered_set>
#include <uchar.h>

class SimulatorManager : public QObject, public TimeManager::TimeListener
{
    Q_OBJECT
public:    
    enum State {
        Running,
        Paused,
        Stopped
    };

    SimulatorManager();
    ~SimulatorManager();

    static void PerformSimulation(SimulationConfiguration simulation_config);

    int getElapsedMonths() { return m_elapsed_months; }

    void setMonthlyTriggerFrequency(int p_frequency);

    virtual void trigger();

    State getState() { return m_state; }

#ifdef GUI_MODE
    const PlantRenderDataContainer & getPlantRenderingData();
    const EnvironmentSpatialHashMap & getEnvironmentRenderingData();
#endif

    static const int _AREA_WIDTH_HEIGHT;//10000; //10000 // Centimeters ==> [50m x 50m]

public slots:
    void start(SimulationConfiguration configuration);
    void pause();
    void resume();
    void stop();
    void generateSnapshot();
    void generateStatisticalSnapshot();

signals:
    void updated(int);
    void newPlant(QString name, QColor color);
    void removedPlant(QString name, QString cause_of_death);

private:
//    void remove_plant(Plant p);
    void add_plant(Plant p_plant);

    EnvironmentManager m_environment_mgr;

    TimeManager m_time_keeper;

#ifdef GUI_MODE
    void refresh_rendering_data();
    PlantRenderDataContainer m_plant_rendering_data;
#endif

    SimulationConfiguration m_configuration;

    PlantStorage m_plant_storage;

    QString plant_status_to_string(Plant::PlantStatus status);

    int m_elapsed_months;
    std::atomic<bool> m_stopping;
    State m_state;

    PlantFactory m_plant_factory;

    std::thread * m_snapshot_creator_thread;
    std::thread * m_statistical_snapshot_thread;

    // Configuration
    bool m_seeding_enabled;
};

#endif //SIMULATOR_MANAGER_H
