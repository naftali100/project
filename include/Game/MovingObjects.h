#pragma once

#include "Timer.h"
#include "Game/Entity.h"
#include <cmath>
#include "SfmlUtil.h"

class MovingObjects : public Entity {
public:
    void setDirection(const sf::Vector2f& v) {
        m_direction = sf::util::normalize(v);
    }

    void setSpeed(float s) {
        m_speed = s;
    }

    // that to do when timeout
    // virtual void timeoutAction() = 0;

    virtual bool isTimeout() = 0;
    // bomb and gifts handle collision in the same way
    void handleCollision(Entity* e, const sf::Vector3f& manifold) override;

    virtual ~MovingObjects() = default;

protected:
    sf::Vector2f m_direction;  // < normalized vector
    bool m_isSelected;
    MyTimer m_timer;
    float m_speed;
    bool m_isTimeOut = false;

private:
    void resolveCollision(const sf::Vector3f& manifold);

};