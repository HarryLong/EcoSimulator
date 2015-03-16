#include "environment_spatial_hashmap.h"
#include <QPoint>

/*********************
 * ILLUMINATION CELL *
 *********************/
IlluminationCell::IlluminationCell(Range p_illumination_range) :
    ResourceCell(p_illumination_range), max_height(.0f), id_to_height()
{
}

IlluminationCell::~IlluminationCell()
{

}

/**********************
 * SOIL HUMIDITY CELL *
 **********************/
SoilHumidityCell::SoilHumidityCell(Range p_humidity_range) :
    ResourceCell(p_humidity_range), requests(), grants()
{

}

SoilHumidityCell::~SoilHumidityCell()
{

}

/********************
 * TEMPERATURE CELL *
 ********************/
TemperatureCell::TemperatureCell(Range temperature_range) :
    ResourceCell(temperature_range)
{

}

TemperatureCell::~TemperatureCell()
{

}

/************************************
 * ENVIRONMENT SPATIAL HASHMAP CELL *
 ************************************/
EnvironmentSpatialHashMapCell::EnvironmentSpatialHashMapCell() : illumination_cell(NULL), soil_humidity_cell(NULL), temp_cell(NULL) {}

EnvironmentSpatialHashMapCell::~EnvironmentSpatialHashMapCell()
{
    if(illumination_cell)
        delete illumination_cell;
    if(soil_humidity_cell)
        delete soil_humidity_cell;
    if(temp_cell)
        delete temp_cell;
}

void EnvironmentSpatialHashMapCell::setMonth(int p_month)
{
    illumination_cell->setMonth(p_month);
    soil_humidity_cell->setMonth(p_month);
    temp_cell->setMonth(p_month);
}

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
