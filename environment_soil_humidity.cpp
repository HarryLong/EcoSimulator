#include "environment_soil_humidity.h"
#include "math.h"

EnvironmentSoilHumidity::EnvironmentSoilHumidity(EnvironmentSpatialHashMap & map) : m_map(map), m_refresh_required(true)
{
}

void EnvironmentSoilHumidity::setSoilHumidityData(const QImage & p_image)
{
    QImage scaled_image (p_image.scaled(QSize(m_map.getHorizontalCellCount(), m_map.getVerticalCellCount()),
                                        Qt::IgnoreAspectRatio));

    for(int x = 0; x < m_map.getHorizontalCellCount(); x++)
    {
        for(int y = 0; y < m_map.getVerticalCellCount(); y++)
        {
            int humidity_percentage((qBlue(scaled_image.pixel(x,y)) / 255.f) * 100);
            m_map.insertSoilHumidityCell(QPoint(x,y),new SoilHumidityCell(humidity_percentage));
        }
    }
}

int EnvironmentSoilHumidity::getHumidityPercentage(QPoint p_center, float p_roots_size, int p_id)
{
    if(m_refresh_required) // Check if a resource refresh is necessary
    {
        refresh_resource_distribution();
        m_refresh_required = false;
    }

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
    m_refresh_required = true;

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
    m_refresh_required = true;

    std::vector<EnvironmentSpatialHashMapCell*> cells(m_map.getCells(p_center, p_roots_size));

    for(auto it (cells.begin()) ; it != cells.end(); it++)
    {
        SoilHumidityCell* cell ((*it)->soil_humidity_cell);

        cell->requests.erase(p_id);
        cell->grants.erase(p_id);
    }
}

void EnvironmentSoilHumidity::refresh_resource_distribution()
{
    for(int x = 0; x < m_map.getHorizontalCellCount(); x++)
    {
        for(int y = 0; y < m_map.getVerticalCellCount(); y++)
        {
            SoilHumidityCell * cell (m_map.get(QPoint(x,y))->soil_humidity_cell);
            if(cell->requests.size() == 0) // No requests
                continue;

            std::vector<ResourceUsageRequest> requests;
            int n_requests(0);
            float remaining_total_size(.0f);
            int remaining_resource_available(cell->humidity_percentage);

            /*
             * The request multiplier increases the amount taken in lower humidity ranges
             * and lowers the amount in the higher ranges as follows:
             * For a humidity of
             *   100% --> 0% will be taken
             *   0% --> 100% of requested will be taken
             */
            float effect_on_environment_multiplier ((100-cell->humidity_percentage)/100.0f);
            int aggregate_environmental_impact(0); // This is the aggregated impact on the enviornment (i.e the amount requested multiplied by the multiplier above)

            for(auto it(cell->requests.begin()); it != cell->requests.end(); it++)
            {
                requests.push_back(it->second);
                remaining_total_size += it->second.size;
                aggregate_environmental_impact += it->second.requested_amount*effect_on_environment_multiplier;
                n_requests++;
            }

            GrantsMap grants;
            /*
             * More resources than necessary: Give each requestee the amount requested + the overflow amount
             */
            if(aggregate_environmental_impact < remaining_resource_available)
            {
                int overflow(remaining_resource_available-aggregate_environmental_impact);
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
                    float vigor(.0f);
                    if(remaining_total_size == .0f) // Can only happen if plants are still seeds
                        vigor = 1.f/n_requests;
                    else
                        vigor = ( it->size / remaining_total_size );
                    int granted_amount ( std::min(it->requested_amount, (int)(vigor * remaining_resource_available) ) );

                    grants.insert(std::pair<int,int>(it->requestee_id,granted_amount ));

                    remaining_total_size -= it->size;
                    remaining_resource_available -= granted_amount*effect_on_environment_multiplier ;
                }
            }
            cell->grants = grants;
        }
    }
}
