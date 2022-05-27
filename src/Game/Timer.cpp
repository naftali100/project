#include "Game/Timer.h"

MyTimer::MyTimer(Func function, const int delay)
    :m_function(function)
{
    setTime(sf::seconds(delay));
}

void MyTimer::setTime(const sf::Time& t){
    *this += t;
}

void MyTimer::set(Func function, const int seconds)
{
    m_function = function;
    setTime(sf::seconds(seconds));
}

void MyTimer::update(const sf::Time& dt){
    *this-= dt;
    if (*this < sf::seconds(0))
        m_function();
}

//void MyTimer::stop() {
//    m_isActive = false;
//}