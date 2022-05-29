#pragma once

#include "Game/MovingObjects.h"

class Gift : public MovingObjects {
public:
    Gift() {
        setSpeed(200);
        m_sprite.setTexture(TextureHolder::get(Textures::Gift));

        m_timer.set([this]() { m_isTimeOut = true; }, 10);  // TODO: calc delay
        m_anim.initFramesWithFixedSize(TextureHolder::get(Textures::Gift).getSize(), 2, 2, 0.1);
        m_anim.setFrame(0);

        setSize(sf::Vector2f(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height));
        setOrigin(getSize() / 2.f);
    }

    void handleEvent(const sf::Event& e) override {
        Entity::handleEvent(e);
        switch (e.type) {
            case sf::Event::MouseButtonReleased:
                if (getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                    takeGift();
                }
                break;
        }
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
