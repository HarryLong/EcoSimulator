#ifndef ENVIRONMENT_SPATIAL_HASHMAP_H
#define ENVIRONMENT_SPATIAL_HASHMAP_H

#include "spatial_hashmap.h"

typedef std::pair<int,int> Range;

/*********************
 * ILLUMINATION CELL *
 *********************/
class IlluminationCell {
public:
    IlluminationCell();
    ~IlluminationCell();

    int getIllumination(int months_elapsed);
    float getMaxHeight();

private:
    std::unordered_map<int, float> id_to_height;
    Range range;
    int increments;
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
struct SoilHumidityCell {
    int humidity_percentage;
    RequestsMap requests;
    GrantsMap grants;

    SoilHumidityCell(int humidity_percentage) : humidity_percentage(humidity_percentage), requests(),grants() {}
};

/********************
 * TEMPERATURE CELL *
 ********************/
struct TemperatureCell{
    int temp;

    TemperatureCell(int temp) : temp(temp) {}
};

/*******************************
 * ENVIRONMENT SPATIAL HASHMAP *
 *******************************/
struct EnvironmentSpatialHashMapCell {
    IlluminationCell * illumination_cell;
    SoilHumidityCell * soil_humidity_cell;
    TemperatureCell * temp_cell;

    EnvironmentSpatialHashMapCell() : illumination_cell(NULL), soil_humidity_cell(NULL), temp_cell(NULL) {}

    ~EnvironmentSpatialHashMapCell()
    {
        if(illumination_cell)
            delete illumination_cell;
        if(soil_humidity_cell)
            delete soil_humidity_cell;
        if(temp_cell)
            delete temp_cell;
    }
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
