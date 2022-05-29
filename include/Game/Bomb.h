#pragma once
#ifndef __BOMB_H__
#define __BOMB_H__

#include "Game/MovingObjects.h"
#include "Log.h"
#include "Resources.h"

class Bomb : public MovingObjects {
public:
    explicit Bomb(bool & isGameOver);
    using MovingObjects::m_timer;

    void update(const sf::Time& dt) override;
    void handleEvent(const sf::Event& e) override;    

    sf::FloatRect getGlobalBounds() const override {
        return getTransform().transformRect(m_sprite.getGlobalBounds());
    }

private:
    bool m_first = false;
    bool& m_isGameOver;
    bool m_isDragged = false;
};
#endif  // __BOMB_H__
