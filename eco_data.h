#ifndef ECO_DATA_H
#define ECO_DATA_H

#include <string>
#include <unordered_set>
#include <vector>
#include "helper.h"
#include <map>
#include <set>

#include <QRgb>

enum SoilType{
    Clay,
    Sandy,
    Silt
};

struct SpeciesRequirements{
    float water;
    float illumination;
    float ph;
    float temperature;

    SpeciesRequirements() : water(.0f), illumination(.0f), ph(.0f), temperature(.0f) {}
};

#endif // ECO_DATA_H
