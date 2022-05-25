#pragma once

#include "Game/MovingObjects.h"

class Gift : public MovingObjects {
public:
    Gift() = default;
    //using MovingObjects::m_isTimeOut;
    virtual bool isTimeout() override {
        return m_isTimeOut;
    };

    void handleEvent(const sf::Event& e) override {
        Entity::handleEvent(e);
    }
    void update(const sf::Time& dt) override {
        // check for click
    }

    void runAnimation() {
        // run animation
    }

private:
    sf::Sprite m_sprite;
    sf::Texture m_texture;
    // Animation m_animation{m_sprite};
};