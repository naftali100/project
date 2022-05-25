#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
using Func = std::function<void()>;

class MyTimer {
    std::atomic<bool> active{ true };

public:
    void setTimeout(Func function, int delay);
    void setInterval(Func function, int interval);
    void stop();

};
