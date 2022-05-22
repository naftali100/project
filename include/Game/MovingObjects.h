#pragma once

#include "Game/Entity.h"

class MovingObjects : public Entity {
public:
    
    // that to do when timeout
    // virtual void timeoutAction() = 0;


    virtual bool isTimeout() = 0;
    // bomb and gifts handle collision in the same way
    virtual void handleCollision(Entity* e) override {

    };

    virtual ~MovingObjects() = default;

protected:
    sf::Vector2f m_direction; // < normalized vector
    bool m_isSelected;
    // timer m_timer
    float m_speed; 
private:
};