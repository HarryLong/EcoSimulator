#ifndef ENVIRONMENT_SPATIAL_HASHMAP_H
#define ENVIRONMENT_SPATIAL_HASHMAP_H

#include "spatial_hashmap.h"
#include <math.h>

typedef std::pair<int,int> Range;

class ResourceCell
{
public:
    ResourceCell(Range range) : range(range), value(0), month(0)
    {
        increments = (range.second - range.first) / 6.0f;
    }

    void setMonth(int month)
    {
        value = std::min((int) (range.second - (std::abs(6-month) * increments)), range.second);
    }

    int get()
    {
        return get(month);
    }

    int get(int p_month)
    {
        if(p_month != month)
        {
            month = p_month;
            value = std::min((int) (range.second - (std::abs(6-month) * increments)), range.second);
        }
        return value;
    }

protected:
    float increments;
    Range range;
    int month;
    int value;
};

/*********************
 * ILLUMINATION CELL *
 *********************/
class IlluminationCell : public ResourceCell {
public:
    IlluminationCell(Range p_illumination_range);
    ~IlluminationCell();

    std::unordered_map<int, float> id_to_height;
    float max_height;
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
class SoilHumidityCell : public ResourceCell{
public:
    SoilHumidityCell(Range p_humidity_range);
    ~SoilHumidityCell();

    RequestsMap requests;
    GrantsMap grants;
};

/********************
 * TEMPERATURE CELL *
 ********************/
class TemperatureCell : public ResourceCell{
public:
    TemperatureCell(Range temperature_range);
    ~TemperatureCell();
};

/*******************************
 * ENVIRONMENT SPATIAL HASHMAP *
 *******************************/
class EnvironmentSpatialHashMapCell {
public:
    IlluminationCell * illumination_cell;
    SoilHumidityCell * soil_humidity_cell;
    TemperatureCell * temp_cell;

    EnvironmentSpatialHashMapCell();

    ~EnvironmentSpatialHashMapCell();

    void setMonth(int p_month);
};

class EnvironmentSpatialHashMap : public SpatialHashMap<EnvironmentSpatialHashMapCell>
{
public:
    EnvironmentSpatialHashMap();
    ~EnvironmentSpatialHashMap();
    void insertIlluminationCell(QPoint point, IlluminationCell * illumination_cell);
    void insertSoilHumidityCell(QPoint point, SoilHumidityCell * illumination_cell);
    void insertTempCell(QPoint point, TemperatureCell * illumination_cell);
};


#endif //ENVIRONMENT_SPATIAL_HASHMAP_H
