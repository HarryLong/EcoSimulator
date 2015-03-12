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
#include "plant_factory.h"

#include "debuger.h"

#include "plant.h"

#include "simulation_configuration.h"

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

    virtual void trigger();

    SimulationState getState() { return m_state; }

    const PlantRenderDataContainer & getPlantRenderingData();
    const EnvironmentSpatialHashMap & getEnvironmentRenderingData();

public slots:
    void start(SimulationConfiguration configuration);
    void pause();
    void resume();
    void stop();

signals:
    void update(void);
    void newPlant(QString name,const QColor * color);
    void removedPlant(QString name, QString cause_of_death);

private:
    void remove_plant(Plant * p);
    void refresh_rendering_data();
    void add_plant(Plant * p_plant);

    EnvironmentManager m_environment_mgr;

    TimeManager m_time_keeper;

    PlantRenderDataContainer m_plant_rendering_data;

    PlantStorage m_plant_storage;

    QString plant_status_to_string(PlantStatus status);

    int m_elapsed_months;
    std::atomic<bool> m_stopping;
    SimulationState m_state;

    PlantFactory m_plant_factory;

    // Configuration
    SimulationOptions * m_simulation_options;

    // TEMPORARY
    QPoint generate_random_position();
};

#endif //SIMULATOR_MANAGER_H
