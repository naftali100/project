#ifndef __TIMER_H__
#define __TIMER_H__
#pragma once

class Timer : public sf::Time {
public:
    Timer() = default;
    Timer(const Func& function, const float delay);
    void setTime(const sf::Time&);
    // set ths timer to 0
    void reset();
    void set(const Func& function, const float);
    void update(const sf::Time&);
    void setInterval(const Func& function, float interval);
    void stop();

    void pause();
    void resume();

private:
    Func m_function;
    bool m_isPaused = false;
};

#endif  // __TIMER_H__