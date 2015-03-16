#include "environment_soil_humidity.h"
#include "math.h"

EnvironmentSoilHumidity::EnvironmentSoilHumidity(EnvironmentSpatialHashMap & map) : m_map(map)
{
}

void EnvironmentSoilHumidity::setSoilHumidityData(PixelData * p_data, int variance)
{
    if(p_data->m_width != m_map.getHorizontalCellCount() ||
            p_data->m_height != m_map.getVerticalCellCount())
    {
        p_data->scale(m_map.getHorizontalCellCount(), m_map.getVerticalCellCount());
    }

    for(int x = 0; x < m_map.getHorizontalCellCount(); x++)
    {
        for(int y = 0; y < m_map.getVerticalCellCount(); y++)
        {
            int min(p_data->getValue(QPoint(x,y)));
            m_map.insertSoilHumidityCell(QPoint(x,y),new SoilHumidityCell(Range(min, min+variance)));
        }
    }
}

int EnvironmentSoilHumidity::getHumidityPercentage(QPoint p_center, float p_roots_size, int p_id)
{
    std::vector<EnvironmentSpatialHashMapCell*> cells(m_map.getCells(p_center, p_roots_size));

    int aggregated_humidity_percentage(0);

    for(auto it (cells.begin()) ; it != cells.end(); it++)
    {
        SoilHumidityCell* cell ((*it)->soil_humidity_cell);
        if(cell->grants.find(p_id) != cell->grants.end())
            aggregated_humidity_percentage += cell->grants.find(p_id)->second;
    }
    return aggregated_humidity_percentage/cells.size(); // Return percentage
}

// DO NOT CALL THIS METHOD FOLLOWED BY GETHUMIDITY CONTRINUOUSLY RATHER UPDATE THIS FOR ALL NECESSARY CELLS IN ONE GO
void EnvironmentSoilHumidity::setData(QPoint p_center, float p_roots_size, int p_id, int p_minimum_humidity_percentage)
{
    std::vector<EnvironmentSpatialHashMapCell*> cells(m_map.getCells(p_center, p_roots_size));

    for(auto it (cells.begin()) ; it != cells.end(); it++)
    {
        SoilHumidityCell* cell ((*it)->soil_humidity_cell);
        auto it2 ( cell->requests.find(p_id) );
        if(it2 != cell->requests.end())
            it2->second.size = p_roots_size;
        else
            cell->requests.insert(std::pair<int,ResourceUsageRequest>(p_id, ResourceUsageRequest(p_minimum_humidity_percentage, p_roots_size, p_id)));
    }
}

void EnvironmentSoilHumidity::remove(QPoint p_center, float p_roots_size, int p_id)
{
    std::vector<EnvironmentSpatialHashMapCell*> cells(m_map.getCells(p_center, p_roots_size));

    for(auto it (cells.begin()) ; it != cells.end(); it++)
    {
        SoilHumidityCell* cell ((*it)->soil_humidity_cell);

        cell->requests.erase(p_id);
        cell->grants.erase(p_id);
    }
}

void EnvironmentSoilHumidity::refresh_resource_distribution(int month)
{
    for(int x = 0; x < m_map.getHorizontalCellCount(); x++)
    {
        for(int y = 0; y < m_map.getVerticalCellCount(); y++)
        {
            SoilHumidityCell * cell (m_map.get(QPoint(x,y))->soil_humidity_cell);
            if(cell->requests.size() > 0)
            {
                GrantsMap grants;

                if(cell->get() == 100) // standing water. All plants get 100% humidity
                {
                    for(auto it(cell->requests.begin()); it != cell->requests.end(); it++)
                    {
                        grants.insert(std::pair<int,int>(it->second.requestee_id, 100));
                    }
                }
                else
                {
                    std::vector<ResourceUsageRequest> requests;
                    int n_requests(0);
                    float remaining_total_vigor(.0f);
                    int humidity_available(cell->get(month));
                    int total_requested_humidity(0);

                    for(auto it(cell->requests.begin()); it != cell->requests.end(); it++)
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
                            grants.insert(std::pair<int,int>(it->requestee_id,granted_amount ));
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

                            grants.insert(std::pair<int,int>(it->requestee_id,granted_amount ));

                            remaining_total_vigor -= it->size;
                            humidity_available -= granted_amount ;
                        }
                    }
                }
                cell->grants = grants;
            }
        }
    }
}
