#include "environment_spatial_hashmap.h"
#include <QPoint>

/*********************
 * ILLUMINATION CELL *
 *********************/
IlluminationCell::IlluminationCell() : max_height(.0f), id_to_height(),
        daily_illumination(p_daily_illumination) {}

IlluminationCell::~IlluminationCell()
{


    int getIllumination(int months_elapsed);
    float getMaxHeight();

    std::unordered_map<int, float> id_to_height;
    Range range;
    int increments;
    float max_height;
};

/*******************************
 * ENVIRONMENT SPATIAL HASHMAP *
 *******************************/
EnvironmentSpatialHashMap::EnvironmentSpatialHashMap()
{
}

EnvironmentSpatialHashMap::~EnvironmentSpatialHashMap()
{
}

void EnvironmentSpatialHashMap::insertIlluminationCell(QPoint point, IlluminationCell * illumination_cell)
{
    if(!get(point))
        init_cell(point);

    get(point)->illumination_cell = illumination_cell;
}

void EnvironmentSpatialHashMap::insertSoilHumidityCell(QPoint point, SoilHumidityCell * soil_humidity_cell)
{
    if(!get(point))
        init_cell(point);

    get(point)->soil_humidity_cell = soil_humidity_cell;
}


void EnvironmentSpatialHashMap::insertTempCell(QPoint point, TemperatureCell * temp_cell)
{
    if(!get(point))
        init_cell(point);

    get(point)->temp_cell = temp_cell;
}
