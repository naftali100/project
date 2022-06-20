#pragma once

#include "Animation.h"
#include "Game/MovingObjects.h"
#include "MessageBus.h"
#include "Resources.h"

class Gift : public MovingObjects {
public:
    Gift(const sf::Vector2f& pos, const sf::Vector2f& dir);

    void handleEvent(const sf::Event& e) override;
    void update(const sf::Time& dt) override;

private:
    void takeGift();
    const MessageType getRandGift() const;

    Animation m_giftAnimation{m_sprite};
    bool m_taken = false;

    sf::Sound m_giftSound{SoundBufferHolder::get(SoundEffect::Gift)};
};
