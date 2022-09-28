#include "Game/Timer.h"

#include "Random.h"

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

void Timer::setTimeout(const Func& function, const float seconds) {
    m_isPaused = false;
    m_function = function;
    setTime(sf::seconds(seconds));
}

void Timer::setInterval(const Func& function, float interval) {
    setTimeout(
        [this, function, interval]() {
            function();
            setTime(sf::seconds(interval));
        },
        interval);
}

void Timer::setRandomInterval(const Func& function, float intervalMin, float intervalMax){
    setTimeout(
        [this, function, intervalMin, intervalMax]() {
            function();
            setTime(sf::seconds(Random::rnd(intervalMin, intervalMax)));
        },
        Random::rnd(intervalMin, intervalMax));
}

void Timer::update(const sf::Time& dt) {
    if (!m_isPaused) {
        *this -= dt;
        if (*this <= sf::seconds(0) && m_function)
            m_function();
    }
}

void Timer::pause() {
    m_isPaused = true;
}

void Timer::resume() {
    m_isPaused = false;
}
