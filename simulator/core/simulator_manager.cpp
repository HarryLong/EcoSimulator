#include "simulator_manager.h"

#include <iostream>
#include <algorithm>
#include <mutex>

#include "../../utils/utils.h"
#include "../plants/plant.h"
#include "../../utils/callback_listener.h"

#include <QDebug>

#include <chrono>
typedef std::chrono::high_resolution_clock Clock;


static QRgb s_black_color_rgb(QColor(Qt::GlobalColor::black).rgb());
const int SimulatorManager::_AREA_WIDTH_HEIGHT = 10000;
SimulatorManager::SimulatorManager() : m_time_keeper(),
    m_plant_storage(SimulatorManager::_AREA_WIDTH_HEIGHT, SimulatorManager::_AREA_WIDTH_HEIGHT),
    m_environment_mgr(SimulatorManager::_AREA_WIDTH_HEIGHT, SimulatorManager::_AREA_WIDTH_HEIGHT),
    m_plant_factory(SimulatorManager::_AREA_WIDTH_HEIGHT, SimulatorManager::_AREA_WIDTH_HEIGHT),
    m_elapsed_months(0), m_state(Stopped), m_snapshot_creator_thread(nullptr), m_statistical_snapshot_thread(nullptr),
    m_stopping(false)
{
    m_time_keeper.addListener(this);
}

SimulatorManager::~SimulatorManager()
{
    if(m_snapshot_creator_thread)
    {
        m_snapshot_creator_thread->join();
        delete m_snapshot_creator_thread;
    }
    if(m_statistical_snapshot_thread)
    {
        m_statistical_snapshot_thread->join();
        delete m_statistical_snapshot_thread;
    }
}

void SimulatorManager::add_plant(Plant p_plant)
{
    if(!m_plant_storage.isPlantAtLocation(p_plant.m_center_position))
    {
        m_plant_storage.add(p_plant);
        m_environment_mgr.updateEnvironment(p_plant.m_center_position, p_plant.getCanopyWidth(), p_plant.getHeight(), p_plant.getRootSize(),
                                            p_plant.m_unique_id, p_plant.getMinimumSoilHumidityRequirement()); // Update resources in environment

        emit newPlant(p_plant.m_specie_name, p_plant.getColor());
    }
}

//void SimulatorManager::remove_plant(Plant p)
//{
//    m_plant_storage.remove(p);
//}

void SimulatorManager::setConfiguration(SimulationConfiguration configuration)
{
    m_configuration = configuration;
    m_environment_mgr.setEnvironmentProperties(configuration.m_slope,
                                               configuration.m_humidity,
                                               configuration.m_illumination,
                                               configuration.m_temperature);

    for(auto specie_it(configuration.m_plants_to_generate.begin()); specie_it != configuration.m_plants_to_generate.end(); specie_it++)
    {
        int specie_id(specie_it->first);
        int plant_count(specie_it->second);
        if(plant_count == -1) // Seeding quantity
            plant_count = m_plant_factory.getSpecieProperties(specie_id).seeding_properties.seed_count;

        for(int i(0); i < plant_count; i++)
            add_plant(m_plant_factory.generate(specie_id));
    }
}

#ifdef GUI_MODE
void SimulatorManager::start( SimulationConfiguration configuration)
{
    setConfiguration(configuration);

//    m_state = Running;

//    while(true)
//        trigger();

    m_state = Running;
    m_stopping.store(false);
    m_time_keeper.start();
}
#else
void SimulatorManager::start( SimulationConfiguration configuration, ProgressListener* progress_listener)
{

    SimulatorManager sm;
    sm.setConfiguration(configuration);
    auto start(Clock::now());
    int elapsed_months(0);
    while((elapsed_months = sm.getElapsedMonths()) < configuration.m_duration)
    {
        sm.trigger();
        progress_listener->progressUpdate((elapsed_months*100.f)/configuration.m_duration);
    }

    progress_listener->progressUpdate("Generating statistical snapshot... This can take some time.");

    sm.generateStatisticalSnapshot();
    auto time(std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count());
    qCritical() << "SIMULATION TIME --> " << time << "ms";
    progress_listener->complete();
}
#endif

void SimulatorManager::resume()
{
    m_state = Running;
    m_stopping.store(false);
    m_time_keeper.start();
}

void SimulatorManager::pause()
{
    m_state = Paused;
    m_stopping.store(true);
    m_time_keeper.stop();
}

void SimulatorManager::stop()
{
    m_state = Stopped;
    m_stopping.store(true);
    m_time_keeper.stop();

    std::this_thread::sleep_for(std::chrono::milliseconds(250)); // Wait a delay to ensure all data processing has stopped from the previous trigger

#ifdef GUI_MODE
    m_plant_rendering_data.lock();
    m_plant_rendering_data.clear();
    m_plant_rendering_data.unlock();
#endif

    m_plant_storage.clear();
    m_environment_mgr.reset();
    m_elapsed_months = 0;
    emit updated(0);
}

void SimulatorManager::trigger()
{
#ifdef GUI_MODE
    if(m_stopping.load())
        return;
#endif
    auto start(Clock::now());
    m_elapsed_months++;

    /*
     * ITERATION # 1:
     * - Based on environmental factors calculates the strength of each plant
     * - Checks the status of the plant and acts accordingly (dies or grows)
     */
    int month((m_elapsed_months%12) + 1);

    m_environment_mgr.setMonth(month);

    // Update all the plants
    std::vector<Plant> surviving_plants;
    std::vector<Plant> deceased_plants;
    m_plant_storage.update(m_environment_mgr, surviving_plants, deceased_plants);

    // Update the environment
    for(Plant & p : surviving_plants)
    {
        m_environment_mgr.updateEnvironment(p.m_center_position, p.getCanopyWidth(), p.getHeight(),
                                            p.getRootSize(), p.m_unique_id, p.getMinimumSoilHumidityRequirement());
    }
    for(Plant & p : deceased_plants)
    {
        m_environment_mgr.updateEnvironment(p.m_center_position, p.getCanopyWidth(), p.getHeight(),
                                            p.getRootSize(), p.m_unique_id, p.getMinimumSoilHumidityRequirement());
        m_environment_mgr.remove(p.m_center_position, p.getCanopyWidth(), p.getRootSize(), p.m_unique_id);
        emit removedPlant(p.m_specie_name, plant_status_to_string(p.getStatus()));
    }

    // Seeding
    if(m_configuration.m_seeding_enabled && m_elapsed_months % 12 == 6)
    {
        std::set<int> species(m_plant_storage.getSpecieIds());

        for(int specie_id : species)
        {
            int specie_seed_count(m_plant_factory.getSpecieProperties(specie_id).seeding_properties.seed_count);
            if(m_plant_storage.containsSpecie(specie_id)) // Use existing plants to seed
            {
                std::vector<Plant> seeding_plants(m_plant_storage.getOnePlantPerCell(specie_id));

                auto plant_it(seeding_plants.begin());

                assert(plant_it != seeding_plants.end());

                int seed_count(0);
                while(seed_count++ < specie_seed_count)
                {
                    Plant & seeding_plant (*plant_it);
                    QPoint position (seeding_plant.seed(1).at(0));
                    if(position.x() >= 0 && position.x() < SimulatorManager::_AREA_WIDTH_HEIGHT &&
                        position.y() >= 0 && position.y() < SimulatorManager::_AREA_WIDTH_HEIGHT)
                    {
                        add_plant(m_plant_factory.generate(specie_id, position));
                    }

                    if(++plant_it == seeding_plants.end())
                        plant_it = seeding_plants.begin();
                }
            }
            else // spawn new plants
            {
                int n_planted(0);
                const SpecieProperties specie_properties (m_plant_factory.getSpecieProperties(specie_id));
                if(specie_properties.illumination_properties.min_illumination == 0 && m_elapsed_months > 240)
                {
                    // shade loving - spawn half at existing plant locations (shaded)
                    std::vector<Plant> plants (m_plant_storage.getPlants());
                    if(plants.size() > 0)
                    {
                        for(; n_planted < specie_seed_count/2; n_planted++)
                        {
                            // Select a random plant
                            Plant & random_plant(*(plants.begin() + (rand()%plants.size())));
                            QPoint location(Utils::getRandomPointInCircle(random_plant.m_center_position,
                                                                                std::max(1.0f,random_plant.getCanopyWidth()/2.f)));
                            if(location.x() < _AREA_WIDTH_HEIGHT && location.y() < _AREA_WIDTH_HEIGHT)
                                add_plant(m_plant_factory.generate(specie_id, location));
                        }
                    }
                }
                for(; n_planted < specie_seed_count; n_planted++)
                    add_plant(m_plant_factory.generate(specie_id));
            }
        }
    }

#ifdef GUI_MODE
    refresh_rendering_data();
#endif
    emit updated(month);    
    auto time(std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count());

    // PLANT COUNT BASED TIMING
    {
        int key(m_plant_storage.getPlantCount()/1000.f);
        auto it(_plant_count_based_timing.find(key));
        if(it == _plant_count_based_timing.end())
        {
            TimeAndCount time_and_count;
            time_and_count.time = time;
            time_and_count.count = 1;
            _plant_count_based_timing.emplace(key, time_and_count);
        }
        else
        {
            it->second.time += time;
            it->second.count++;
        }
    }

//    // PLANTS PER MONTH
//    {
//        _plant_count_per_month.emplace(m_elapsed_months, m_plant_storage.getPlantCount());
//    }

//    // ELAPSED TIME BASED TIMING
//    {
//        _elapsed_months_based_timing.emplace(m_elapsed_months, time);
//    }

//    // PLANT AVERAGE SIZE
//    {
//        std::map<int, float> specie_avg_sizes;
//        std::map<int, int> specie_count;

//        for(int specie_id : m_plant_storage.getSpecieIds())
//        {
//            specie_avg_sizes[specie_id] = 0;
//            specie_count[specie_id] = 0;
//        }

//        std::vector<Plant> plants(m_plant_storage.getPlants());
//        for(Plant & p : plants)
//        {
//            specie_avg_sizes[p.m_specie_id] += p.getCanopyWidth();
//            specie_count[p.m_specie_id]++;
//        }

//        for(auto it(specie_count.begin()); it != specie_count.end(); it++)
//        {
//            _specie_average_size[it->first].push_back(((float)specie_avg_sizes[it->first]) / it->second);
//        }
//    }

    if(m_elapsed_months == 1200)
    {
//        std::cout << "******************SPECIE AVERAGE SIZE*************" << std::endl;
//        for(auto it(_specie_average_size.begin()); it != _specie_average_size.end(); it++)
//        {
//            std::cout << "SPECIE --> " << it->first << std::endl;
//            for(int i(0); i < it->second.size(); i++)
//                std::cout << (i+1) << " , " << it->second.at(i) << std::endl;

//        }
        std::cout << "******************PLANT COUNT BASED*************" << std::endl;
        for(auto it(_plant_count_based_timing.begin()); it != _plant_count_based_timing.end(); it++)
            std::cout << it->first << ", " << (it->second.time/(float)it->second.count) << std::endl;
////        std::cout << "******************MONTH BASED*************" << std::endl;
////        for(auto it(_elapsed_months_based_timing.begin()); it != _elapsed_months_based_timing.end(); it++)
////            std::cout << it->first << ", " << _plant_count_per_month[it->first] << ", " << it->second << std::endl;
    }
}

#ifdef GUI_MODE
const PlantRenderDataContainer& SimulatorManager::getPlantRenderingData()
{
    return m_plant_rendering_data;
}

const EnvironmentSpatialHashMap & SimulatorManager::getEnvironmentRenderingData()
{
    return m_environment_mgr.getRenderingData();
}
#endif

void SimulatorManager::setMonthlyTriggerFrequency(int p_frequency)
{
    m_time_keeper.setUnitTime(p_frequency);
}

QString SimulatorManager::plant_status_to_string(Plant::PlantStatus status)
{
    switch (status){
        case Plant::PlantStatus::DeathByAge:
            return QString("Age");
        case Plant::PlantStatus::DeathByUnderIllumination:
            return  QString("Insufficient sunlight");
        case Plant::PlantStatus::DeathByOverIllumination:
            return  QString("Too much sunlight");
        case Plant::PlantStatus::DeathByFlood:
            return  QString("Flooded");
        case Plant::PlantStatus::DeathByDrought:
            return  QString("Drought");
        case Plant::PlantStatus::DeathByCold:
            return QString("Cold");
        case Plant::PlantStatus::DeathByHeat:
            return QString("Heat");
        case Plant::PlantStatus::DeathBySlope:
            return QString("Slope");
    default:
        return QString("This is a bug!");
    }
}

#ifdef GUI_MODE
void SimulatorManager::refresh_rendering_data()
{
    m_plant_rendering_data.lock();

    m_plant_rendering_data.clear();

    for(Plant & p : m_plant_storage.getSortedPlants(SortingCriteria::Height))
    {
        m_plant_rendering_data.push_back( PlantRenderingData(p.m_specie_name, p.getColor(), p.m_center_position, p.getHeight(), p.getCanopyWidth(), p.getRootSize()));
    }

    m_plant_rendering_data.unlock();
}
#endif

void SimulatorManager::generateSnapshot()
{
    if(m_snapshot_creator_thread)
    {
        m_snapshot_creator_thread->join();
        delete m_snapshot_creator_thread;
    }

    m_snapshot_creator_thread = new std::thread(&PlantStorage::generateSnapshot, &m_plant_storage, true);
}

void SimulatorManager::generateStatisticalSnapshot()
{
    m_plant_storage.generateStatisticalSnapshot(m_environment_mgr.getSlope(), m_environment_mgr.getHumidities(), m_environment_mgr.getIlluminations(),
                                                m_environment_mgr.getTemperatures(), m_elapsed_months);
}

void SimulatorManager::generateStatisticalSnapshot(CallbackListener * completion_listener)
{
    if(m_statistical_snapshot_thread)
    {
        m_statistical_snapshot_thread->join();
        delete m_statistical_snapshot_thread;
    }

    m_statistical_snapshot_thread =
            new std::thread(&PlantStorage::generateStatisticalSnapshot, &m_plant_storage, m_environment_mgr.getSlope(), m_environment_mgr.getHumidities(), m_environment_mgr.getIlluminations(),
                    m_environment_mgr.getTemperatures(), m_elapsed_months, completion_listener, true);
}
