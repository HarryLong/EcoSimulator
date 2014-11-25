#ifndef ENVIRONMNENT_SOIL_HUMIDITY_H
#define ENVIRONMNENT_SOIL_HUMIDITY_H

#include "spatial_hashmap.h"
#include <QPoint>

struct ResourceUsageRequest{
    int requested_amount;
    float size;
    int requestee_id;

    ResourceUsageRequest(int p_requested_amount, float p_size, int p_requestee_id) :
        requested_amount(p_requested_amount), size(p_size), requestee_id(p_requestee_id) {}
};

typedef std::unordered_map<int, ResourceUsageRequest> RequestsMap;
typedef std::unordered_map<int, int> GrantsMap;

struct SoilHumidityCellContent {
    int humidity_percentage;
    RequestsMap requests;
    GrantsMap grants;

    SoilHumidityCellContent() : humidity_percentage(40), requests(),grants() {}
};
typedef SpatialHashMap<SoilHumidityCellContent> SoilHumiditySpatialHashMap;
class EnvironmentSoilHumidity
{
public:
    EnvironmentSoilHumidity();

    int getHumidityPercentage(QPoint p_center, float p_radius, int p_id);
    void setData(QPoint p_center, float p_radius, int p_id, int p_minimum_humidity_percentage);

    void remove(QPoint p_center, float p_radius, int p_id);
    void reset();
    SoilHumiditySpatialHashMap getSoilHumiditySpatialHashmap() { return m_spatial_hashmap; }

protected:
    void init();

private:
    void refresh_resource_distribution();
    SoilHumiditySpatialHashMap m_spatial_hashmap;
    bool m_refresh_required;
};

#endif //ENVIRONMNENT_SOIL_HUMIDITY_H
