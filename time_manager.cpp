#include "time_manager.h"
#include <time.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include "boost/foreach.hpp"
#include "constants.h"

TimeManager::TimeManager() : m_unit_time(-1), m_listeners()
{
}

TimeManager::~TimeManager()
{
    if(m_time_keeper != NULL)
    {
        m_time_keeper->join();
        delete m_time_keeper;
    }
}

void TimeManager::start()
{
    if(m_unit_time != -1)
    {
        m_run = true;
        m_time_keeper = new std::thread(&TimeManager::process_one_unit_time, this);
    }
    else
        std::cerr << "First you must set a unit time!" << std::endl;
}

void TimeManager::stop()
{
    m_run.store(false);
    setUnitTime(0); // This will ensure the current thread stops
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME*2)); // Ensure the timer has stopped
}

void TimeManager::setUnitTime(int p_unit_time_ms)
{
    std::cout << "Setting unit time to: " << p_unit_time_ms << std::endl;
    m_unit_time.store(p_unit_time_ms);
}

void TimeManager::addListener(TimeListener* p_listener)
{
    m_listeners.push_back(p_listener);
}

/***********
 * PRIVATE *
 ***********/
void TimeManager::process_one_unit_time()
{
    using namespace std::chrono;

    auto t_start = high_resolution_clock::now();
    std::chrono::milliseconds sleep_time(SLEEP_TIME);

    high_resolution_clock::time_point t_end;

    do{
        std::this_thread::sleep_for(sleep_time);
        t_end = t_start + std::chrono::milliseconds(m_unit_time.load());
    }while(std::chrono::high_resolution_clock::now() < t_end);

    unit_time_complete_callback();
}

void TimeManager::unit_time_complete_callback()
{
    BOOST_FOREACH(TimeListener* l, m_listeners)
    {
        l->trigger();
    }
    if(m_run) // continue to run
        start();
}
