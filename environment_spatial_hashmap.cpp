#include "environment_spatial_hashmap.h"
#include <QPoint>

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
