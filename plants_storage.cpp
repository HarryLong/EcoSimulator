#include "plants_storage.h"
#include "plant.h"
#include <algorithm>
#include "boost/foreach.hpp"
#include <QImage>
#include <QPainter>
#include <QTemporaryDir>
#include "tracker.h"
#define LOCATION_STORAGE_CELL_SIZE 100

#include "analysis_point.h"

PlantStorage::PlantStorage() : m_plants(), m_plant_count(0),
  m_location_plant_accessor(LOCATION_STORAGE_CELL_SIZE, LOCATION_STORAGE_CELL_SIZE, std::ceil(((float)AREA_WIDTH_HEIGHT)/LOCATION_STORAGE_CELL_SIZE),
                            std::ceil(((float)AREA_WIDTH_HEIGHT)/LOCATION_STORAGE_CELL_SIZE)),
  m_statistical_analyzer_config(0, 500, 20, AREA_WIDTH_HEIGHT, AREA_WIDTH_HEIGHT)
{


}

PlantStorage::~PlantStorage()
{
    clear();
}

void PlantStorage::add(Plant * p_plant)
{
    // Ensure specie exists in data collection
    auto it(m_plants.find(p_plant->m_specie_id));
    if(it == m_plants.end()) // Specie not yet inserted
    {
        m_plants.insert(std::pair<int, std::map<long, Plant*>>(p_plant->m_specie_id, std::map<long, Plant*>()));
    }
    // Insert the specie
    m_plants[p_plant->m_specie_id].insert(std::pair<long, Plant*>(p_plant->m_unique_id, p_plant));

    // Location based
    LocationCell * cell(m_location_plant_accessor.getCell(p_plant->m_center_position, true));
    if(cell->species.find(p_plant->m_specie_id) == cell->species.end())
        cell->species.insert(std::pair<int,std::map<Position,Plant*> >(p_plant->m_specie_id, std::map<Position,Plant*>()));
    cell->species[p_plant->m_specie_id].insert(std::pair<Position, Plant*>(Position(p_plant->m_center_position.x(), p_plant->m_center_position.y()), p_plant));

    m_plant_count++;
}

void PlantStorage::remove(Plant * p_plant)
{
    if(contains(p_plant))
    {
        m_plants[p_plant->m_specie_id].erase(p_plant->m_unique_id);
        // Location based
        LocationCell * cell(m_location_plant_accessor.getCell(p_plant->m_center_position));
        cell->species[p_plant->m_specie_id].erase(Position(p_plant->m_center_position.x(), p_plant->m_center_position.y()));

        delete p_plant;
        m_plant_count--;
    }
}

bool PlantStorage::contains(Plant * p_plant) const
{
    auto it(m_plants.find(p_plant->m_specie_id));
    return it != m_plants.end() && it->second.find(p_plant->m_unique_id) != it->second.end();
}

std::vector<Plant*> PlantStorage::getOnePlantPerCell(int p_specie_id)
{
    std::vector<Plant*> ret;
    auto it(m_plants.find(p_specie_id));
    if(it != m_plants.end()) // No plants of this specie
    {
        std::vector<std::map<Position, Plant*> > relevant_cells;

        for(auto location_cell(m_location_plant_accessor.begin()); location_cell != m_location_plant_accessor.end(); location_cell++)
        {
            auto plants(location_cell->second.species.find(p_specie_id));
            if(plants != location_cell->second.species.end() && plants->second.size() > 0)
                relevant_cells.push_back(plants->second);
        }

        std::sort(relevant_cells.begin(), relevant_cells.end(), [](std::map<Position, Plant*> lhs, std::map<Position, Plant*> rhs) {return lhs.size() < rhs.size();});

        for(std::map<Position, Plant*> plant_cell : relevant_cells)
        {
            auto random_plant(plant_cell.begin());
            std::advance(random_plant, rand() % plant_cell.size());
            ret.push_back(random_plant->second);
        }
    }

    return ret;
}

std::vector<Plant*> PlantStorage::getPlants()
{
    std::vector<Plant*> all_plants;

    for(auto specie(m_plants.begin()); specie!= m_plants.end(); specie++)
    {
        auto specie_plants(specie->second);
        for(auto plant(specie_plants.begin()); plant != specie_plants.end(); plant++)
            all_plants.push_back(plant->second);
    }
    return all_plants;
}

std::vector<Plant*> PlantStorage::getSortedPlants(SortingCriteria p_sorting_criteria)
{
    std::vector<Plant*> plants(getPlants());

    switch(p_sorting_criteria)
    {
    case SortingCriteria::Strength:
        std::sort(plants.begin(), plants.end(), [](Plant * lhs, Plant* rhs) {return lhs->getHeight() < rhs->getHeight();});
        break;
    case SortingCriteria::Height:
        std::sort(plants.begin(), plants.end(), [](Plant * lhs, Plant* rhs) {return lhs->getVigor() < rhs->getVigor();});
        break;
    }
    return plants;
}

void PlantStorage::clear()
{
    for(Plant * p : getPlants())
        delete p;

    m_plants.clear();
    m_location_plant_accessor.clear();
}

const PlantStorageStructure& PlantStorage::getPlantsBySpecies()
{
    return m_plants;
}

int PlantStorage::getPlantCount()
{
    return m_plant_count;
}

bool PlantStorage::isPlantAtLocation(QPoint p_location)
{
    LocationCell * cell (m_location_plant_accessor.getCell(p_location));
    if(cell == NULL) // No plants in cell
        return false;

    std::map<int, std::map<std::pair<int,int>, Plant*> > species_in_cell (cell->species);
    std::pair<int,int> location(p_location.x(), p_location.y());

    for(auto it(species_in_cell.begin()); it != species_in_cell.end(); it++)
        if(it->second.find(location) != it->second.end())
            return true;

    return false;
}

void PlantStorage::lock()
{
    m_storage_accessor_mutex.lock();
}

void PlantStorage::unlock()
{
    m_storage_accessor_mutex.unlock();
}

#define SNAPSHOTS_FOLDER "/home/harry/snapshots/snapshot_"
void PlantStorage::generateSnapshot()
{
    lock();

    std::vector<QImage*> images;
    std::vector<QPainter*> painters;

    for(int i = 0; i < m_plants.size()+1; i++)
    {
        QImage * img(new QImage(AREA_WIDTH_HEIGHT, AREA_WIDTH_HEIGHT, QImage::Format_RGB32));
        img->fill(Qt::GlobalColor::black);
        QPainter * painter(new QPainter(img));
        images.push_back(img);
        painters.push_back(painter);
    }

    QPainter * base_painter(painters[0]);
    int specie_idx(1);
    for(auto specie(m_plants.begin()); specie != m_plants.end(); specie++, specie_idx++)
    {
        QPainter * specie_painter = painters[specie_idx];
        for(auto plant(specie->second.begin()); plant != specie->second.end(); plant++)
        {
            Plant * p = plant->second;

            specie_painter->setBrush(*(p->m_color.get()));
            base_painter->setBrush(*(p->m_color.get()));

            int radius(std::max(1,(int)std::round(p->getCanopyWidth()/2.0f)));
            specie_painter->drawEllipse(p->m_center_position,radius,radius);
            base_painter->drawEllipse(p->m_center_position,radius,radius);
        }
        specie_painter->end();
    }
    base_painter->end();

    specie_idx = 0;
    for(QImage * img : images)
    {
        QString filename(SNAPSHOTS_FOLDER);
        filename.append(specie_idx == 0 ? "all" : QString::number(specie_idx));
        filename.append(".png");
        img->save(filename);
        delete img;
        specie_idx++;
    }

    for(QPainter * p : painters)
    {
        delete p;
    }

    std::cout << "Snapshot created!" << std::endl;
    unlock();
}

void PlantStorage::generateStatisticalSnapshot( std::pair<int,int> humidity, std::pair<int,int> illumination, std::pair<int,int> temperature,
                                                int elapsed_months)
{
    QTemporaryDir tmp_dir;
    if(tmp_dir.isValid())
    {
        std::map<float,int> avg_height_to_specie_id;
        // Create analysis points
        std::map<int, std::vector<AnalysisPoint*>> specie_analysis_points;
        lock();
        for(auto specie(m_plants.begin()); specie != m_plants.end(); specie++)
        {
            int specie_id(specie->first);
            float avg_height = 0;
            std::vector<AnalysisPoint*> analysis_points;
            for(auto plant(specie->second.begin()); plant != specie->second.end(); plant++)
            {
                Plant * p = plant->second;
                avg_height += p->getHeight();
                analysis_points.push_back(new AnalysisPoint(specie_id, p->m_center_position, std::max(1.0f,p->getCanopyWidth()/2.0f)));
            }
            avg_height /= specie->second.size();
            avg_height_to_specie_id.insert(std::pair<float,int>(avg_height, specie_id));
            specie_analysis_points.insert(std::pair<int, std::vector<AnalysisPoint*>>(specie_id, analysis_points));
        }
        unlock();

        std::vector<int> priority_sorted_category_ids;
        for(auto it(avg_height_to_specie_id.rbegin()); it != avg_height_to_specie_id.rend(); it++)
        {
            std::cout << it->first << " > ";
            priority_sorted_category_ids.push_back(it->second);
        }
        m_statistical_analyzer_config.setPrioritySortedCategoryIds(priority_sorted_category_ids);

        Analyzer::analyze(tmp_dir.path(), specie_analysis_points, m_statistical_analyzer_config);

        Tracker::setNewData(humidity, illumination, temperature, elapsed_months, getSpecieIds(), tmp_dir);
    }
    else
    {
        std::cerr << "Failed to create temporary directory..." << std::endl;
    }
}

const std::set<int> PlantStorage::getSpecieIds()
{
    std::set<int> specie_ids;
    for(auto it(m_plants.begin()); it != m_plants.end(); it++)
        specie_ids.insert(it->first);
    return specie_ids;
}
