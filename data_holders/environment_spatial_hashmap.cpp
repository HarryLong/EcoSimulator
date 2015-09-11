#include "environment_spatial_hashmap.h"
#include <QPoint>

#define SPATIAL_HASHMAP_CELL_WIDTH 25 // Centimeters
#define SPATIAL_HASHMAP_CELL_HEIGHT 25 // Centimeters

/*********************
 * ILLUMINATION CELL *
 *********************/
int IlluminationCell::_total_available_illumination = 0;
IlluminationCell::IlluminationCell() : id_to_height()
{
    reset();
}

IlluminationCell::~IlluminationCell()
{

}

void IlluminationCell::update(int p_id, float p_height)
{
    id_to_height[p_id] = p_height;
    reset();
}

void IlluminationCell::reset()
{
    m_max_height = -1;
    m_max_height_id = -1;
}

int IlluminationCell::getIllumination(int p_id, int p_height)
{
    if(m_max_height_id == -1) // Refresh required
        refresh();

    if(p_height > m_max_height || m_max_height_id == p_id) // Plants that don't block shade are not stored. Just check if they are above the first shade blocking plant
        return IlluminationCell::_total_available_illumination;

    return 0;
}

#include <QDebug>
void IlluminationCell::refresh()
{
    int i(0);
    for(auto it(id_to_height.begin()); it != id_to_height.end(); it++, i++)
    {
        if(i == 0 || it->second > m_max_height)
        {
            m_max_height_id = it->first;
            m_max_height = it->second;
        }
    }
}

void IlluminationCell::remove(int p_id)
{
    id_to_height.erase(p_id);
    reset();
}

int IlluminationCell::getRenderingIllumination() const
{
    return (id_to_height.size() > 0 ? 0 : IlluminationCell::_total_available_illumination);
}

/**********************
 * SOIL HUMIDITY CELL *
 **********************/
int SoilHumidityCell::_total_available_humidity = 0;
int SoilHumidityCell::id_incrementor = 0;
SoilHumidityCell::SoilHumidityCell() : m_requests(), m_grants(), m_refresh_required(true), m_unique_id(id_incrementor++)
{

}

SoilHumidityCell::~SoilHumidityCell()
{

}

void SoilHumidityCell::reset()
{
    m_refresh_required = true;
    m_grants.clear();
}

int SoilHumidityCell::getGrantedHumidity(int p_id)
{
    if(m_refresh_required) // Refresh requited
        refresh();

    auto it(m_grants.find(p_id));

    if(it != m_grants.end())
        return it->second;

    return 0;
}

void SoilHumidityCell::update(int p_id, float p_roots_size,int p_minimum_humidity)
{
    ResourceUsageRequest request(p_minimum_humidity, p_roots_size, p_id);
    auto it(m_requests.find(p_id));
    if(it != m_requests.end())
        it->second = request;
    else
        m_requests.emplace(p_id, request);

    m_refresh_required = true;
}

void SoilHumidityCell::remove(int p_id)
{
    m_requests.erase(p_id);

    m_refresh_required = true;
}

void SoilHumidityCell::refresh()
{
    m_grants.clear();
    int humidity_available( SoilHumidityCell::_total_available_humidity );

    if(humidity_available >= 300) //  No splitting necessary --> Water plentiful
    {
        for(auto it(m_requests.begin()); it != m_requests.end(); it++)
        {
            m_grants.emplace(it->second.requestee_id, humidity_available);
        }
    }
    else
    {
        std::vector<ResourceUsageRequest> requests;
        int n_requests(0);
        float remaining_total_vigor(.0f);
        int total_requested_humidity(0);

        for(auto it(m_requests.begin()); it != m_requests.end(); it++)
        {
            requests.push_back(it->second);
            remaining_total_vigor += it->second.size;
            total_requested_humidity += it->second.requested_amount;
            n_requests++;
        }

        /*
         * More resources than necessary: Give each requestee the amount requested + the overflow amount
         */
        if(total_requested_humidity < humidity_available)
        {
            int overflow(humidity_available-total_requested_humidity);
            for(auto it(requests.begin()); it != requests.end(); it++)
            {
                int granted_amount (it->requested_amount + overflow);
                m_grants.emplace(it->requestee_id,granted_amount );
            }
        }
        /*
         * Less resources than necessary: Split based on vigor as follows:
         *  1. Iterate trough requests by vigor (most vigorous first)
         *  2. For each request, grant the minimum of the requested amount and (vigor/total_vigor) * remaining available ResourceUsageRequest
         */
        else
        {
            // First sort the requests based on vigor
            std::sort(requests.begin(), requests.end(), [](const ResourceUsageRequest & lhs, const ResourceUsageRequest & rhs) {return lhs.size < rhs.size;});
            // Iterate in reverse order
            for(auto it(requests.rbegin()); it != requests.rend(); it++)
            {
                float vigor( it->size / remaining_total_vigor );
                int granted_amount ( std::min(it->requested_amount, (int)(vigor * humidity_available) ) );

                m_grants.emplace( it->requestee_id, granted_amount );

                remaining_total_vigor -= it->size;
                humidity_available -= granted_amount ;
            }
        }
    }
    m_refresh_required = false;
}

int SoilHumidityCell::getRenderingHumidity() const
{
    return (m_grants.size() > 0 ? 0 : SoilHumidityCell::_total_available_humidity);
}

/********************
 * TEMPERATURE CELL *
 ********************/
int TemperatureCell::_temperature = 0;
TemperatureCell::TemperatureCell()
{

}

TemperatureCell::~TemperatureCell()
{

}

int TemperatureCell::getTemperature() const
{
    return _temperature;
}

/************************************
 * ENVIRONMENT SPATIAL HASHMAP CELL *
 ************************************/
EnvironmentSpatialHashMapCell::EnvironmentSpatialHashMapCell() :
    illumination_cell(), soil_humidity_cell(), temp_cell()
{

}

EnvironmentSpatialHashMapCell::~EnvironmentSpatialHashMapCell()
{
//    if(illumination_cell)
//        delete illumination_cell;
//    if(soil_humidity_cell)
//        delete soil_humidity_cell;
}

/*******************************
 * ENVIRONMENT SPATIAL HASHMAP *
 *******************************/
EnvironmentSpatialHashMap::EnvironmentSpatialHashMap(int area_width, int area_height) :
    SpatialHashMap<EnvironmentSpatialHashMapCell>(SPATIAL_HASHMAP_CELL_WIDTH, SPATIAL_HASHMAP_CELL_HEIGHT,
                                                 std::ceil(((float)area_width)/SPATIAL_HASHMAP_CELL_WIDTH),
                                                 std::ceil(((float)area_height)/SPATIAL_HASHMAP_CELL_HEIGHT))
{
//    for(int x = 0; x < getHorizontalCellCount(); x++)
//    {
//        for(int y = 0; y < getVerticalCellCount(); y++)
//        {
//            init_cell(QPoint(x,y));
//            EnvironmentSpatialHashMapCell * cell = get(QPoint(x,y));
//            cell->soil_humidity_cell = new SoilHumidityCell;
//            cell->illumination_cell = new IlluminationCell;
//            cell->temp_cell = new TemperatureCell;
//        }
//    }
}

EnvironmentSpatialHashMap::~EnvironmentSpatialHashMap()
{

}

std::vector<QPoint> EnvironmentSpatialHashMap::getPoints(QPoint p_center, float p_radius)
{
    return SpatialHashMap<EnvironmentSpatialHashMapCell>::getPoints(p_center, p_radius, true);
}

void EnvironmentSpatialHashMap::setAvailableResources(int p_available_illumination, int p_available_humidity, int p_temperature)
{
    SoilHumidityCell::_total_available_humidity = p_available_humidity;
    IlluminationCell::_total_available_illumination = p_available_illumination;
    TemperatureCell::_temperature = p_temperature;
    resetAllCells();
}

void EnvironmentSpatialHashMap::resetAllCells()
{
    for(auto it(begin()); it != end(); it++)
    {
        it->second.soil_humidity_cell.reset();
        it->second.illumination_cell.reset();
    }
//    clear();
//    for(int x = 0; x < getHorizontalCellCount(); x++)
//    {
//        for(int y = 0; y < getVerticalCellCount(); y++)
//        {
//            EnvironmentSpatialHashMapCell & cell ( this->operator [](QPoint(x,y) ));
//            cell.soil_humidity_cell.reset();
//            cell.illumination_cell.reset();
//        }
//    }
}
