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

    bool isTimeout() override {
        return m_isTimeOut;
    };

    void update(const sf::Time& dt) override;

    sf::FloatRect getGlobalBounds() const override {
        return getTransform().transformRect(m_sprite.getGlobalBounds());
    }

    void handleEvent(const sf::Event& e) override{
        // 
    };
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(m_sprite, states);
    }

private:
    bool m_first = false;
    sf::Sprite m_sprite;
    bool& m_isGameOver;

    //void explode();
};
#endif  // __BOMB_H__