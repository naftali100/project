#pragma once

#include "Game/MovingObjects.h"
#include "Animation.h"

class Gift : public MovingObjects {
public:
    Gift();

    void handleEvent(const sf::Event& e) override;
    void update(const sf::Time& dt) override {
        m_timer.update(dt);

        MovingObjects::update(dt);
    }

    void takeGift() {
        // run take animation and wait for it
        m_isTimeOut = true;
    }

private:
    Animation m_anim{m_sprite};
};
