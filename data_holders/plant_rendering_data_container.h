#ifndef PLANT_RENDERING_DATA_CONTAINER_H
#define PLANT_RENDERING_DATA_CONTAINER_H

#include "plant_rendering_data.h"

#include <vector>
#include <mutex>

struct PlantRenderDataContainer : public std::vector<PlantRenderingData>
{
public:
    PlantRenderDataContainer() : mutex() {}

    void lock() const
    {
        mutex.lock();
    }

    void unlock() const
    {
        mutex.unlock();
    }

private:
    mutable std::mutex mutex;
};

#endif // PLANT_RENDERING_DATA_CONTAINER_H
