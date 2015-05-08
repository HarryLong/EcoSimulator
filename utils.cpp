#include "utils.h"
#include <math.h>

QPoint RandomUtils::getRandomPointInCircle(QPoint center, int radius)
{
    int distance(rand() % radius);
    float angle_in_radians((((float)rand())/RAND_MAX) * 2 * M_PI);

    QPoint diff(cos(angle_in_radians) * distance, sin(angle_in_radians) * distance);

    return QPoint(center + diff);
}


