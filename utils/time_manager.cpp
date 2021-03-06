#include "time_manager.h"
#include <time.h>
#include <chrono>
#include <ctime>
#include <iostream>

#define SLEEP_TIME 20

TimeManager::TimeManager() : m_unit_time(-1), m_listeners(), m_time_keeper(NULL)
{
}

TimeManager::~TimeManager()
{
    stop();
    if(m_time_keeper)
    {
        m_time_keeper->join();
        delete m_time_keeper;
    }
}

void TimeManager::start()
{
    if(m_unit_time.load() != -1)
    {
        m_stop.store(false);
        m_time_keeper = new std::thread(&TimeManager::process_one_unit_time, this);
    }
    else
        std::cerr << "First you must set a unit time!" << std::endl;
}

void TimeManager::stop()
{
    m_stop.store(true);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME*2)); // Ensure the timer has stopped
}

void TimeManager::setUnitTime(int p_unit_time_ms)
{
    m_unit_time.store(p_unit_time_ms);
}

void TimeManager::addListener(TimeManager::TimeListener * p_listener)
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
    }while(!m_stop.load() && std::chrono::high_resolution_clock::now() < t_end);

    if(!m_stop.load())
        unit_time_complete_callback();
}

void TimeManager::unit_time_complete_callback()
{
    callback_listeners();
    if(!m_stop.load()) // continue to run
        start();
}

void TimeManager::callback_listeners()
{
    for(TimeManager::TimeListener * l : m_listeners)
        l->trigger();
}
