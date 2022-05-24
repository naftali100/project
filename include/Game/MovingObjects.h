#pragma once

#include "Game/Entity.h"
// #include <math.h>
#include <cmath>

class MovingObjects : public Entity {
public:
    void setDirection(const sf::Vector2f& v) {
        m_direction = normalize(v);
    }

    void setSpeed(float s){
        m_speed = s;
    }

    // that to do when timeout
    // virtual void timeoutAction() = 0;

    virtual bool isTimeout() = 0;
    // bomb and gifts handle collision in the same way
    virtual void handleCollision(Entity* e) override{

    };

    virtual ~MovingObjects() = default;

protected:
    sf::Vector2f m_direction;  // < normalized vector
    bool m_isSelected;
    // timer m_timer
    float m_speed;

private:
    template <typename T>
    sf::Vector2<T> normalize(const sf::Vector2<T>& v) {
        float length = std::sqrt((v.x * v.x) + (v.y * v.y));
        if (length != 0)
            return sf::Vector2<T>(v.x / length, v.y / length);
        else
            return v;
    }
};