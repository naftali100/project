#pragma once

#include "Game/MovingObjects.h"

class Gift : public MovingObjects {
public:
    Gift() {
        setSpeed(200);
        m_sprite.setTexture(TextureHolder::get(Textures::Stars));
        setSize(sf::Vector2f(TextureHolder::get(Textures::Stars).getSize()));
        setOrigin(getSize() / 2.f);
        m_timer.set([this]() { m_isTimeOut = true; }, 5);  // TODO: calc delay
    }
    using MovingObjects::m_isTimeOut;

    // virtual bool isTimeout() override {
    //     return m_isTimeOut;
    // };

    void handleEvent(const sf::Event& e) override {
        Entity::handleEvent(e);
        if (e.MouseButtonPressed)
            takeGift();
    }
    void update(const sf::Time& dt) override {
        m_timer.update(dt);
        move(m_direction * m_speed * dt.asSeconds());
    }

    void takeGift() {
        // run take animation and wait for it
        m_isTimeOut = true;
    }

private:
    // sf::Sprite m_sprite;
    sf::Texture m_texture;
};
