#include "environment_soil_humidity.h"
#include "math.h"

EnvironmentSoilHumidity::EnvironmentSoilHumidity() : m_refresh_required(true), data_set(false)
{
}
void EnvironmentSoilHumidity::setSoilHumidityData(const QImage* p_image)
{
    QImage scaled_image (p_image->scaled(QSize(m_spatial_hashmap.getHorizontalCellCount(), m_spatial_hashmap.getVerticalCellCount()),
                                        Qt::IgnoreAspectRatio));

    for(int x = 0; x < m_spatial_hashmap.getHorizontalCellCount(); x++)
    {
        for(int y = 0; y < m_spatial_hashmap.getVerticalCellCount(); y++)
        {
            int humidity_percentage((qBlue(scaled_image.pixel(x,y)) / 255.f) * 100);
            m_spatial_hashmap.insert(QPoint(x,y),SoilHumidityCellContent(humidity_percentage));
        }
    }
    data_set = true;
}

int EnvironmentSoilHumidity::getHumidityPercentage(QPoint p_center, float p_radius, int p_id)
{
    if(m_refresh_required) // Check if a resource refresh is necessary
    {
        refresh_resource_distribution();
        m_refresh_required = false;
    }

    BoundingBox bb(m_spatial_hashmap.getBoundingBox(p_center, p_radius));

    int aggregated_humidity_percentage(0);
    int total_cell_count(0);
    for(int x = bb.x_min; x < bb.x_max; x++)
    {
        for(int y = bb.y_min; y < bb.y_max; y++)
        {
            SoilHumidityCellContent * cell_data (m_spatial_hashmap.get(QPoint(x,y)));

            aggregated_humidity_percentage += cell_data->grants.find(p_id)->second;

            total_cell_count++;
        }
    }
    return (aggregated_humidity_percentage/total_cell_count);
}

// DO NOT CALL THIS METHOD FOLLOWED BY GETHUMIDITY CONTRINUOUSLY RATHER UPDATE THIS FOR ALL NECESSARY CELLS IN ONE GO
void EnvironmentSoilHumidity::setData(QPoint p_center, float p_radius, int p_id, int p_minimum_humidity_percentage)
{
    m_refresh_required = true;
    BoundingBox bb(m_spatial_hashmap.getBoundingBox(p_center, p_radius));

    for(int x = bb.x_min; x < bb.x_max; x++)
    {
        for(int y = bb.y_min; y < bb.y_max; y++)
        {
            SoilHumidityCellContent * cell_data (m_spatial_hashmap.get(QPoint(x,y)));

            auto it ( cell_data->requests.find(p_id) );
            if(it != cell_data->requests.end())
                it->second.size = p_radius;
            else
                cell_data->requests.insert(std::pair<int,ResourceUsageRequest>(p_id, ResourceUsageRequest(p_minimum_humidity_percentage, p_radius, p_id)));
        }
    }
}

void EnvironmentSoilHumidity::remove(QPoint p_center, float p_radius, int p_id)
{
    BoundingBox bb(m_spatial_hashmap.getBoundingBox(p_center, p_radius));

    for(int x = bb.x_min; x < bb.x_max; x++)
    {
        for(int y = bb.y_min; y < bb.y_max; y++)
        {
            SoilHumidityCellContent * cell_data (m_spatial_hashmap.get(QPoint(x,y)));
            cell_data->requests.erase(p_id);
            cell_data->grants.erase(p_id);
        }
    }
}

void EnvironmentSoilHumidity::reset()
{
    m_spatial_hashmap.clear();
}

void EnvironmentSoilHumidity::refresh_resource_distribution()
{
    for(int x = 0; x < m_spatial_hashmap.getHorizontalCellCount(); x++)
    {
        for(int y = 0; y < m_spatial_hashmap.getVerticalCellCount(); y++)
        {
            SoilHumidityCellContent * cell_data (m_spatial_hashmap.get(QPoint(x,y)));
            if(cell_data->requests.size() == 0) // No requests
                continue;

            std::vector<ResourceUsageRequest> requests;
            int n_requests(0);
            int n_aggregate_resource_requests(0);
            float remaining_total_size(.0f);
            int remaining_resource_available(cell_data->humidity_percentage);
            for(auto it(cell_data->requests.begin()); it != cell_data->requests.end(); it++)
            {
                requests.push_back(it->second);
                remaining_total_size += it->second.size;
                n_aggregate_resource_requests += it->second.requested_amount;
                n_requests++;
            }

            GrantsMap grants;
            /*
             * More resources than necessary: Give each requestee the amount requested, then split the remaining equally.
             */
            if(n_aggregate_resource_requests < remaining_resource_available)
            {
                int remaining_resource_overflow (remaining_resource_available - n_aggregate_resource_requests);
                int overflow_amount_per_requestee (remaining_resource_overflow / n_requests);
                int request_number(0);
                for(auto it(requests.rbegin()); it != requests.rend(); it++)
                {
                    int granted_amount (it->requested_amount);

                    if(++request_number == n_requests)
                        granted_amount += remaining_resource_overflow;
                    else
                    {
                        granted_amount += overflow_amount_per_requestee;
                        remaining_resource_overflow -= overflow_amount_per_requestee;
                    }

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
                    remaining_resource_available -= granted_amount;
                }
            }

            cell_data->grants = grants;
        }
    }
}
