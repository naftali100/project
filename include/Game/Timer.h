#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>


using Func = std::function<void()>;

class MyTimer : public sf::Time {
public:
    void setTime(sf::Time);
    void update(sf::Time);
    void setTimeout(Func function, int delay);
    void setInterval(Func function, int interval);
    void stop();

private:
    std::atomic<bool> active{ true };
};
