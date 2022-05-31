#include "Game/Timer.h"

MyTimer::MyTimer(const Func& function, const float delay) : m_function(function) {
    setTime(sf::seconds(delay));
}

void MyTimer::setTime(const sf::Time& t) {
    *this += t;
    m_isPaused = false;
}

void MyTimer::reset() {
    *this -= sf::milliseconds(asMilliseconds());
}

void MyTimer::set(const Func& function, const float seconds) {
    m_isPaused = false;
    m_function = function;
    setTime(sf::seconds(seconds));
}

void MyTimer::update(const sf::Time& dt) {
    if (!m_isPaused) {
        *this -= dt;
        if (*this < sf::seconds(0))
            m_function();
    }
}

// void MyTimer::stop() {
//     m_isActive = false;
// }

void MyTimer::pause() {
    m_isPaused = true;
}

void MyTimer::resume() {
    m_isPaused = false;
}
