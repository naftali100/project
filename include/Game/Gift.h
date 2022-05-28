#pragma once

#include "Game/MovingObjects.h"

class Gift : public MovingObjects {
public:
    Gift() {
        setSpeed(200);
        m_sprite.setTexture(TextureHolder::get(Textures::Stars));
        setSize(sf::Vector2f(TextureHolder::get(Textures::Stars).getSize()));
        setOrigin(getSize() / 2.f);
        m_timer.set([this]() { m_isTimeOut = true; }, 10);  // TODO: calc delay
    }

    void handleEvent(const sf::Event& e) override {
        Entity::handleEvent(e);
        if (e.MouseButtonPressed)
            takeGift();
    }

    void update(const sf::Time& dt) override {
        MovingObjects::update(dt);
    }

    void takeGift() {
        // run take animation and wait for it
        m_isTimeOut = true;
    }

private:
    sf::Texture m_texture;
};
