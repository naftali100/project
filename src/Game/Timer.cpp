#include "Game/Timer.h"


void MyTimer::setTime(sf::Time t){
    *this += t;
}

void MyTimer::update(sf::Time dt){
    *this-= dt;
}
void MyTimer::setTimeout(Func function, int delay) {
    active = true;
    std::thread t([=]() {
        if (!active.load()) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if (!active.load()) return;
        function();
        });
    t.detach();
}

void MyTimer::setInterval(Func function, int interval) {
    active = true;
    std::thread t([=]() {
        while (active.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if (!active.load()) return;
            function();
        }
        });
    t.detach();
}

void MyTimer::stop() {
    active = false;
}