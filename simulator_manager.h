#ifndef SIMULATOR_MANAGER_H
#define SIMULATOR_MANAGER_H

#include "uchar.h"
#include "time_manager.h"
#include "display_area.h"
#include "plant_factory.h"

#include <QObject>
#include <vector>
#include <QRgb>
#include <string>
#include "constants.h"
#include "helper.h"
#include <map>
#include <array>

class Plant;

typedef std::map<long, Plant*> plant_map_type;

class SimulatorManager : public QObject, public TimeListener
{
    Q_OBJECT
public:
    SimulatorManager();
    ~SimulatorManager();
    void start();
    DisplayArea * getDisplay() { return m_display; }
    virtual void trigger();
    void addPlant(Plant * p_plant);

public slots:
    void setTimeAcceleration(int p_acceleration);

private:
    TimeManager m_time_keeper;

    DisplayArea * m_display;

    QRgb * m_display_data;
    std::array<std::vector<Plant*>,AREA_WIDTH*AREA_HEIGHT> m_world_plant_ref;

    std::map<long,Plant*> m_plants;

    PlantFactory m_plant_factory;

    void remove_plant_data(Plant* p_plant);

    // TEMPORARY
    void generate_random_plants();
    Coordinate toCoord(int p_index);
    int toIndex(Coordinate p_coord);
    int get_random_index();
    bool check_position_valid(Coordinate c);
    bool is_shadowed(const Plant * p_plant, const Coordinate & p_coord);
};

#endif //SIMULATOR_MANAGER_H
