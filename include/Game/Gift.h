#pragma once

#include "Game/MovingObjects.h"

class Gift : public MovingObjects {
public:
    //Gift() = default;
    Gift::Gift()
    {
        setSpeed(200);
        m_sprite.setTexture(TextureHolder::get(Textures::Stars));
        setSize(sf::Vector2f(TextureHolder::get(Textures::Stars).getSize()));
        setOrigin(getSize() / 2.f);
        m_timer.set([&]() {m_isTimeOut = true; }, 10); //TODO: calc delay
    }
    using MovingObjects::m_isTimeOut;

   /* virtual bool isTimeout() override {
        return m_isTimeOut;
    };*/

    void handleEvent(const sf::Event& e) override {
        Entity::handleEvent(e);
    }
    void update(const sf::Time& dt) override {
        // check for click
        move(m_direction * m_speed * dt.asSeconds());
    }



private:
    sf::Sprite m_sprite;
    sf::Texture m_texture;
    // Animation m_animation{m_sprite};
};