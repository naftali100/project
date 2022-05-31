#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>


using Func = std::function<void()>;

class MyTimer : public sf::Time {
public:
    MyTimer() = default;
    MyTimer(const Func& function, const float delay);
    void setTime(const sf::Time&);
    // set ths timer to 0
    void reset(); 
    void set(const Func& function, const float);
    void update(const sf::Time&);
    //void setTimeout(Func function, int delay);
    // TODO: implement interval
    //void setInterval(Func function, int interval);
    void stop();

    void pause();
    void resume();

private:
    //std::atomic<bool> m_isActive{ true };
    Func m_function;
    //sf::Time m_time;
    bool m_isPaused = false;
};
