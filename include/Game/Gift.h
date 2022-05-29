#pragma once

#include "Game/MovingObjects.h"

class Gift : public MovingObjects {
public:
    Gift();

    void handleEvent(const sf::Event& e) override;
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
