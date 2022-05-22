#pragma once

#include "Game/MovingObjects.h"

class Gift: public MovingObjects {
public:

    virtual bool isTimeout() override {};

    void handleEvent(const sf::Event& e) override {
        Entity::handleEvent(e);
    }
    void update(const sf::Time& dt) override {
        // check for click
    }

    void runAnimation(){
        // run animation
    }
private:
    sf::Sprite m_sprite;
    sf::Texture m_texture;
    // Animation m_animation{m_sprite};


};