#include "plants_storage.h"
#include "../../utils/callback_listener.h"

#include <ecotracker/tracker.h>
#include <radialDistribution/analysis_point.h>
#include <radialDistribution/analyser/analyzer.h>

#include <iostream>
#include <algorithm>
#include <QImage>
#include <QPainter>
#include <QTemporaryDir>

#include <QDebug>

#define LOCATION_STORAGE_CELL_SIZE 100

//namespace std {
//  template <>
//  struct hash<QPoint>
//  {
//    std::size_t operator()(const QPoint & t) const
//    {
//      std::size_t val = 0 ;
//      boost::hash_combine(val,t.x());
//      boost::hash_combine(val,t.y());
//      return val;
//    }
//  };
//}

bool operator<(const QPoint & lhs, const QPoint & rhs)
{
    return lhs.x() < rhs.x() || lhs.y() < rhs.y();
}

PlantStorage::PlantStorage(int area_width, int area_height) : m_plants(), m_plant_count(0),
  m_location_queryable_plants(LOCATION_STORAGE_CELL_SIZE, LOCATION_STORAGE_CELL_SIZE, std::ceil(((float)area_width)/LOCATION_STORAGE_CELL_SIZE),
                            std::ceil(((float)area_height)/LOCATION_STORAGE_CELL_SIZE)),
  m_statistical_analyzer_config(0, 500, 20, area_width, area_width),
  m_area_width(area_width), m_area_height(area_height)
{

}

PlantStorage::~PlantStorage()
{
    clear();
}

// NOT THREAD SAFE!!
Plant PlantStorage::operator[](int plant_id) const
{
    auto it(m_plants.find(plant_id));


    if(it != m_plants.end())
    {
        return it->second;
    }

    throw PlantStorage::InvalidPlantIDException();
}

void PlantStorage::update(EnvironmentManager & environment_manager, std::vector<Plant> & surviving_plants, std::vector<Plant> & deceased_plants, bool mutex_lock)
{
    if(mutex_lock)
        lock();
    for(auto it(m_plants.begin()); it!= m_plants.end(); it++)
    {
        Plant & p(it->second);

//        qCritical() << "Updating for plant: " << p.m_specie_name << "(ID: " << p.m_specie_id;
        int illum(environment_manager.getDailyIllumination(p.m_center_position, p.m_unique_id, p.getCanopyWidth(), p.getHeight()));
        int sh(environment_manager.getSoilHumidity(p.m_center_position, p.getRootSize(), p.m_unique_id));
        int temp(environment_manager.getTemperature());

        p.calculateStrength(illum, sh, temp);

        if(p.getStatus() == Plant::PlantStatus::Alive)
        {
//            qCritical() << "Roots radius before: " << p.getRootSize();
            p.newMonth();
//            qCritical() << "Roots radius after " << p.getRootSize() << " [ S: " << p.getVigor();
            surviving_plants.push_back(p);
        }
        else // Dead
        {
            deceased_plants.push_back(p);
        }
    }
    for(Plant & p : deceased_plants)
        remove(p, false);

    if(mutex_lock)
        unlock();
}

void PlantStorage::add(const Plant & p_plant, bool mutex_lock )
{
    if(mutex_lock)
        lock();
//    qCritical() << "*******************************************";
//    qCritical() << "ADDING PLANT [" << p_plant.m_center_position.x() << "," <<
//                                  p_plant.m_center_position.y() << "] --> " << p_plant.m_unique_id;
    // Raw plant storage
    m_plants.emplace(p_plant.m_unique_id, p_plant);

    // By Specie ID
    m_specie_id_queryable_plants[p_plant.m_specie_id].insert(p_plant.m_unique_id);

    // By Location
    LocationCell & cell(m_location_queryable_plants.getCell(p_plant.m_center_position));
    cell.species[p_plant.m_specie_id].emplace(p_plant.m_center_position, p_plant.m_unique_id);

    m_plant_count++;

    if(mutex_lock)
        unlock();
}

void PlantStorage::remove(const Plant & p_plant, bool mutex_lock)
{
    if(contains_plant(p_plant.m_unique_id, mutex_lock))
    {
        if(mutex_lock)
            lock();

//        qCritical() << "*******************************************";
//        qCritical() << "REMOVING PLANT [" << p_plant.m_center_position.x() << "," <<
//                                      p_plant.m_center_position.y() << "] --> " << p_plant.m_unique_id;

        // Raw plant storage
//        qCritical() << "PLANTS CONTAINS --> " << (m_plants.find(p_plant.m_unique_id) == m_plants.end() ? "NO!" : "YES!" );
        m_plants.erase(p_plant.m_unique_id);
        // By Specie ID
//        qCritical() << "PLANTS BY SPECIE CONTAINS --> " <<
//                       ( m_specie_id_queryable_plants[p_plant.m_specie_id].find(p_plant.m_unique_id) ==  m_specie_id_queryable_plants[p_plant.m_specie_id].end() ? "NO!" : "YES!" );
        m_specie_id_queryable_plants[p_plant.m_specie_id].erase(p_plant.m_unique_id);

        // By Location
        LocationCell & cell(m_location_queryable_plants.getCell(p_plant.m_center_position));
//        qCritical() << "PLANTS BY LOCATION CONTAINS ? " << ( cell.species[p_plant.m_specie_id].find(p_plant.m_center_position) == cell.species[p_plant.m_specie_id].end() ? "NO" : "YES" );
        cell.species[p_plant.m_specie_id].erase(p_plant.m_center_position);

        if(mutex_lock)
            unlock();

        m_plant_count--;
    }
}

bool PlantStorage::contains_plant(int plant_id, bool mutex_lock) const
{
    if(mutex_lock)
        lock();
    bool found(m_plants.find(plant_id) != m_plants.end());
    if(mutex_lock)
        unlock();

    return found;
}

bool PlantStorage::containsSpecie(int specie_id, bool mutex_lock) const
{
    auto it(m_specie_id_queryable_plants.find(specie_id));
    bool found ( it != m_specie_id_queryable_plants.end() && it->second.size() > 0);

    return found;
}

std::vector<Plant> PlantStorage::getOnePlantPerCell(int p_specie_id, bool mutex_lock) const
{
    std::vector<Plant> ret;

    if(containsSpecie(p_specie_id, mutex_lock))
    {
        std::vector<std::unordered_map<QPoint, int> > relevant_cells;

        if(mutex_lock)
            lock();
        for(auto location_cell(m_location_queryable_plants.cbegin()); location_cell != m_location_queryable_plants.cend(); location_cell++)
        {
            auto plants(location_cell->second.species.find(p_specie_id));
            if(plants != location_cell->second.species.end() && plants->second.size() > 0 )
            {
                relevant_cells.push_back(plants->second);
            }
        }

        std::sort(relevant_cells.begin(), relevant_cells.end(), [](std::unordered_map<QPoint, int> lhs, std::unordered_map<QPoint, int> rhs) {return lhs.size() < rhs.size();});

        for(std::unordered_map<QPoint, int> & plant_cell : relevant_cells)
        {
            auto random_position(plant_cell.begin());
            std::advance(random_position, rand() % plant_cell.size());
            ret.push_back(this->operator [](random_position->second));
        }

        if(mutex_lock)
            unlock();
    }

    return ret;
}

std::vector<Plant> PlantStorage::getPlants(bool mutex_lock) const
{
    std::vector<Plant> all_plants;

    if(mutex_lock)
        lock();
    for(auto it(m_plants.begin()); it != m_plants.end(); it++)
        all_plants.push_back(it->second);
    if(mutex_lock)
        unlock();

    return all_plants;
}

std::list<Plant> PlantStorage::getSortedPlants(SortingCriteria p_sorting_criteria, bool mutex_lock) const
{
    std::vector<Plant> plants(getPlants(mutex_lock));

    std::list<Plant> ret;
    ret.insert(ret.begin(), plants.begin(), plants.end());

    switch(p_sorting_criteria)
    {
    case SortingCriteria::Strength:
        ret.sort([](const Plant & lhs, const Plant & rhs){ return lhs.getVigor() > rhs.getVigor(); });
        break;
    case SortingCriteria::Height:
        ret.sort([](const Plant & lhs, const Plant & rhs){ return lhs.getHeight() > rhs.getHeight(); });
        break;
    }
    return ret;
}

void PlantStorage::clear(bool mutex_lock)
{
    if(mutex_lock)
        lock();
    m_plants.clear();
    m_specie_id_queryable_plants.clear();
    m_location_queryable_plants.clear();
    if(mutex_lock)
        unlock();
}

PlantStorage::SpecieQueryablePlants PlantStorage::getPlantsBySpecies()
{
    return m_specie_id_queryable_plants;
}

int PlantStorage::getPlantCount() const
{
    return m_plant_count;
}

bool PlantStorage::isPlantAtLocation(QPoint p_location, bool mutex_lock) const
{
    if(mutex_lock)
        lock();

    if(!m_location_queryable_plants.ws_initialised(p_location))
    {
        if(mutex_lock)
            unlock();
        return false;
    }

    const LocationCell & cell (m_location_queryable_plants.getCell(p_location));

    bool found(false);
    for(auto it(cell.species.begin()); it != cell.species.end() && !found; it++)
    {
        if(it->second.find(p_location) != it->second.end())
        {
            found = true;
        }
    }
    if(mutex_lock)
        unlock();

    return found;
}

std::set<int> PlantStorage::getSpecieIds(bool mutex_lock) const
{
    std::set<int> specie_ids;
    if(mutex_lock)
        lock();
    for(auto it(m_specie_id_queryable_plants.begin()); it != m_specie_id_queryable_plants.end(); it++)
        specie_ids.insert(it->first);
    if(mutex_lock)
        unlock();
    return specie_ids;
}

#define SNAPSHOTS_FOLDER "/home/harry/snapshots/snapshot_"
#define BASE_PAINTER_IDX -1
void PlantStorage::generateSnapshot(bool mutex_lock) const
{
    std::set<int> specie_ids(getSpecieIds());
    std::map<int, QImage *> specie_id_to_image;
    std::map<int, QPainter *> specie_id_to_painter;

    // ADD THE BASE PAINTER
    QImage * base_img(new QImage(m_area_width, m_area_height, QImage::Format_RGB32));
    base_img->fill(Qt::GlobalColor::black);
    QPainter * base_painter(new QPainter(base_img));
    for(int specie_id : specie_ids)// -1 is the base painter (all species)
    {
        QImage * img(new QImage(m_area_width, m_area_height, QImage::Format_RGB32));
        img->fill(Qt::GlobalColor::black);
        QPainter * painter(new QPainter(img));
        specie_id_to_image.emplace(specie_id, img);
        specie_id_to_painter.emplace(specie_id, painter);
    }

    if(mutex_lock)
        lock();
    for(auto specie(m_specie_id_queryable_plants.begin()); specie != m_specie_id_queryable_plants.end(); specie++)
    {
        QPainter * specie_painter = specie_id_to_painter[specie->first];
        for(auto plant_id(specie->second.begin()); plant_id != specie->second.end(); plant_id++)
        {
            Plant p (this->operator [](*plant_id));

            specie_painter->setBrush(p.m_color);
            base_painter->setBrush(p.m_color);

            int radius(std::max(1,(int)std::round(p.getCanopyWidth()/2.0f)));
            specie_painter->drawEllipse(p.m_center_position,radius,radius);
            base_painter->drawEllipse(p.m_center_position,radius,radius);
        }
        specie_painter->end();
    }
    base_painter->end();

    if(mutex_lock)
        unlock();

    // BASE
    QString filename(SNAPSHOTS_FOLDER);
    filename.append("all");
    filename.append(".png");
    base_img->save(filename);

    for(int specie_id : specie_ids) // -1 is the base painter (all species)
    {
        QImage * img(specie_id_to_image[specie_id]);
        QString filename(SNAPSHOTS_FOLDER);
        filename.append(specie_id == BASE_PAINTER_IDX ? "all" : QString::number(specie_id));
        filename.append(".png");
        img->save(filename);
    }

    // DELETIONS
    for(auto it(specie_id_to_painter.begin()); it != specie_id_to_painter.end(); it++)
    {
        specie_id_to_painter.erase(it);
        delete it->second;
    }
    for(auto it(specie_id_to_image.begin()); it != specie_id_to_image.end(); it++)
    {
        specie_id_to_image.erase(it);
        delete it->second;
    }

    delete base_img;
    delete base_painter;
    std::cout << "Snapshot created!" << std::endl;
}

void PlantStorage::generateStatisticalSnapshot(std::vector<int> humidities, std::vector<int> illuminations, std::vector<int> temperatures, int elapsed_months,
                                               CallbackListener * work_completion_listener, bool mutex_lock)
{
    QTemporaryDir tmp_dir;
    if(tmp_dir.isValid())
    {
        std::map<float,int> avg_height_to_specie_id;
        // Create analysis points
        std::map<int, std::vector<AnalysisPoint>> specie_analysis_points;
        if(mutex_lock)
            lock();
        for(auto specie(m_specie_id_queryable_plants.begin()); specie != m_specie_id_queryable_plants.end(); specie++)
        {
            int specie_id(specie->first);
            float avg_height = 0;
            std::vector<AnalysisPoint> analysis_points;
            for(auto plant(specie->second.begin()); plant != specie->second.end(); plant++)
            {
                Plant p ( this->operator []( *plant) );
                avg_height += p.getHeight();
                analysis_points.push_back(AnalysisPoint(specie_id, p.m_center_position, std::max(1.0f,p.getCanopyWidth()/2.0f)));
            }
            avg_height /= specie->second.size();
            avg_height_to_specie_id.emplace(avg_height, specie_id);
            specie_analysis_points.emplace(specie_id, analysis_points);
        }
        if(mutex_lock)
            unlock();
        /**********************
         * PREPARE CATEGORIES *
         **********************/
        std::vector<int> priority_sorted_category_ids;
        for(auto it(avg_height_to_specie_id.rbegin()); it != avg_height_to_specie_id.rend(); it++)
            priority_sorted_category_ids.push_back(it->second);

        m_statistical_analyzer_config.setPrioritySortedCategoryIds(priority_sorted_category_ids);

        Analyzer::analyze(tmp_dir.path(), specie_analysis_points, m_statistical_analyzer_config);

        Tracker::addEntry(humidities, illuminations, temperatures, elapsed_months, getSpecieIds(), tmp_dir);
    }
    else
    {
        qCritical() << "Failed to create temporary directory for statistical snapshot...";
    }
    if(work_completion_listener)
        work_completion_listener->complete();
}

void PlantStorage::lock() const
{
    m_storage_accessor_mutex.lock();
}

void PlantStorage::unlock() const
{
    m_storage_accessor_mutex.unlock();
}

