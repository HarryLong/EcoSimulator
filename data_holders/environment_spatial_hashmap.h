#ifndef ENVIRONMENT_SPATIAL_HASHMAP_H
#define ENVIRONMENT_SPATIAL_HASHMAP_H

#include "SpatialHashmap/spatial_hashmap.h"
#include <math.h>

/*********************
 * ILLUMINATION CELL *
 *********************/
class IlluminationCell {
public:
    IlluminationCell();
    ~IlluminationCell();
    void update(int p_id, float p_height);
    void reset();
    int getIllumination(int p_id, int p_height);
    void remove(int p_id);
    int getRenderingIllumination() const;

    static int _total_available_illumination;

private:
    void refresh();
    std::map<int, float> id_to_height;
    float m_max_height;
    int m_max_height_id;
};

/**********************
 * SOIL HUMIDITY CELL *
 **********************/
struct ResourceUsageRequest{
    int requested_amount;
    float size;
    int requestee_id;

    ResourceUsageRequest(int p_requested_amount, float p_size, int p_requestee_id) :
        requested_amount(p_requested_amount), size(p_size), requestee_id(p_requestee_id) {}
};
typedef std::unordered_map<int, ResourceUsageRequest> RequestsMap;
typedef std::unordered_map<int, int> GrantsMap;
class SoilHumidityCell{
public:
    SoilHumidityCell();
    ~SoilHumidityCell();
    void reset();
    int getGrantedHumidity(int p_id);
    void remove(int p_id);
    void update(int p_id, float p_roots_size,int p_minimum_humidity);

    int getRenderingHumidity() const;
    static int _total_available_humidity;

private:
    void refresh();
    RequestsMap m_requests;
    GrantsMap m_grants;
    bool m_refresh_required;
    int m_unique_id;
    static int id_incrementor;
};


/********************
 * TEMPERATURE CELL *
 ********************/
class TemperatureCell {
public:
    TemperatureCell();
    ~TemperatureCell();

    int getTemperature() const;

    static int _temperature;
};

/*******************************
 * ENVIRONMENT SPATIAL HASHMAP *
 *******************************/
class EnvironmentSpatialHashMapCell {
public:
    IlluminationCell illumination_cell;
    SoilHumidityCell soil_humidity_cell;
    TemperatureCell temp_cell;

    EnvironmentSpatialHashMapCell();
    ~EnvironmentSpatialHashMapCell();
};

class EnvironmentSpatialHashMap : public SpatialHashMap<EnvironmentSpatialHashMapCell>
{
public:
    EnvironmentSpatialHashMap(int area_width, int area_height);
    ~EnvironmentSpatialHashMap();
    std::vector<QPoint> getPoints(QPoint p_center, float p_radius) const;
    void setAvailableResources(int p_available_illumination, int p_available_humidity, int p_temperature );
    void resetAllCells();
};

#endif //ENVIRONMENT_SPATIAL_HASHMAP_H
