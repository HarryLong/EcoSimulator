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
class CallbackListener;

struct TimeAndCount{
    int time;
    int count;
};

class SimulatorManager : public QObject, public TimeManager::TimeListener
{
    Q_OBJECT
public:    
    enum State {
        Running,
        Paused,
        Stopped
    };

    class ProgressListener{
    public:
        virtual void progressUpdate(float percent) = 0;
        virtual void progressUpdate(QString info) = 0;
        virtual void complete() = 0;
    };

    SimulatorManager();
    ~SimulatorManager();

    int getElapsedMonths() { return m_elapsed_months; }

    void setMonthlyTriggerFrequency(int p_frequency);

    virtual void trigger();

    State getState() { return m_state; }

#ifdef GUI_MODE
    const PlantRenderDataContainer & getPlantRenderingData();
    const EnvironmentSpatialHashMap & getEnvironmentRenderingData();
#else
static void start(SimulationConfiguration configuration, ProgressListener* progress_listener);
#endif

    static const int _AREA_WIDTH_HEIGHT;//10000; //10000 // Centimeters ==> [50m x 50m]

public slots:
#ifdef GUI_MODE
    void start(SimulationConfiguration configuration);
#endif
    void setConfiguration(SimulationConfiguration configuration);
    void pause();
    void resume();
    void stop();
    void generateSnapshot();
    void generateStatisticalSnapshot(CallbackListener * completion_listener);
    void generateStatisticalSnapshot(); // non-asynchronous
    void generate_rendering_data(bool);

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

    std::atomic<bool> m_generate_rendering_data;
    std::map<int, std::vector<float> > _specie_average_size;
    std::map<int, TimeAndCount> _plant_count_based_timing;
    std::map<int, int> _plant_count_per_month;
    std::map<int, int> _elapsed_months_based_timing;
};

#endif //SIMULATOR_MANAGER_H
