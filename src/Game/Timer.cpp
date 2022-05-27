#include "Game/Timer.h"

MyTimer::MyTimer(const Func& function, const float delay)
    :m_function(function)
{
    setTime(sf::seconds(delay));
}

void MyTimer::setTime(const sf::Time& t){
    *this += t;
}

void MyTimer::set(const Func& function, const float seconds)
{
    m_function = function;
    setTime(sf::seconds(seconds));
}

void MyTimer::update(const sf::Time& dt){
    *this -= dt;
    if (*this < sf::seconds(0))
        m_function();
}

//void MyTimer::stop() {
//    m_isActive = false;
//}