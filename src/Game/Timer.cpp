#include "Game/Timer.h"

Timer::Timer(const Func& function, const float delay) : m_function(function) {
    setTime(sf::seconds(delay));
}

void Timer::setTime(const sf::Time& t) {
    reset();
    *this += t;
    m_isPaused = false;
}

void Timer::reset() {
    *this -= sf::milliseconds(asMilliseconds());
}

void Timer::set(const Func& function, const float seconds) {
    m_isPaused = false;
    m_function = function;
    setTime(sf::seconds(seconds));
}

void Timer::setInterval(Func function, int interval){
    set([this, function, interval](){
        function();
        setTime(sf::seconds(interval)); 
    }, interval);
}

void Timer::update(const sf::Time& dt) {
    if (!m_isPaused) {
        *this -= dt;
        if (*this < sf::seconds(0))
            m_function();
    }
}

// void Timer::stop() {
//     m_isActive = false;
// }

void Timer::pause() {
    m_isPaused = true;
}

void Timer::resume() {
    m_isPaused = false;
}
