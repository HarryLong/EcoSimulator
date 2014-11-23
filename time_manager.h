#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <vector>
#include <atomic>
#include <thread>

#define CLOCKS_PER_MILLISECONDS CLOCKS_PER_SEC/1000

class TimeListener
{
public:
    virtual void newMonth() = 0;
};

class TimeManager
{
public:
    TimeManager();
    ~TimeManager();

    void addListener(TimeListener* p_listener);
    void start();
    void stop();
    void setUnitTime(int p_unit_time_ms);

private:
    std::atomic<int> m_unit_time;
    std::atomic<bool> m_stop;

    std::vector<TimeListener*> m_listeners;
    std::thread * m_time_keeper;

    void process_one_unit_time();
    void unit_time_complete_callback();
};

#endif // TIME_MANAGER_H
