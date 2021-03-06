#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <vector>
#include <atomic>
#include <thread>

#define CLOCKS_PER_MILLISECONDS CLOCKS_PER_SEC/1000

class TimeManager
{
public:
    class TimeListener
    {
    public:
        virtual void trigger() = 0;
    };

    TimeManager();
    ~TimeManager();

    void addListener(TimeManager::TimeListener* p_listener);
    void start();
    void stop();
    void setUnitTime(int p_unit_time_ms);
    static const int _FASTEST;

private:
    std::atomic<int> m_unit_time;
    std::atomic<bool> m_stop;

    std::vector<TimeManager::TimeListener*> m_listeners;
    std::thread * m_time_keeper;

    void process_one_unit_time();
    void unit_time_complete_callback();
    void callback_listeners();
};

#endif // TIME_MANAGER_H
