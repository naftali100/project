#pragma once

#include "Game/MovingObjects.h"
#include "Animation.h"
#include "MessageBus.h"

class Gift : public MovingObjects {
public:
    Gift();

    void handleEvent(const sf::Event& e) override;
    void update(const sf::Time& dt) override;

    void takeGift();

private:
    const MessageType getRandGift() const;

    Animation m_giftAnimation{m_sprite};
    bool m_taken = false;
};
