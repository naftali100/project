#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>


using Func = std::function<void()>;

class MyTimer : public sf::Time {
public:
    MyTimer() = default;
    MyTimer(Func function, const int delay);
    void setTime(const sf::Time&);
    void set(Func function, const int);
    void update(const sf::Time&);
    //void setTimeout(Func function, int delay);
    //void setInterval(Func function, int interval);
    void stop();

private:
    //std::atomic<bool> m_isActive{ true };
    Func m_function;
    //sf::Time m_time;
};
