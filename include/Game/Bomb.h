#pragma once
#ifndef __BOMB_H__
#define __BOMB_H__

#include "Game/MovingObjects.h"
#include "Log.h"
#include "Resources.h"

class Bomb : public MovingObjects {
public:
    Bomb(bool & isGameOver);
    using MovingObjects::m_timer;

    /*virtual bool isTimeout() override {
        return m_isTimeOut;
    };*/

    virtual void update(const sf::Time& dt) override {
        // drag
        // update movement
        move(m_direction * m_speed * dt.asSeconds());
    };

    sf::FloatRect getGlobalBounds() const {
        return getTransform().transformRect(m_sprite.getGlobalBounds());
    }

    virtual void handleEvent(const sf::Event& e) override{};
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(m_sprite, states);
    }

private:
    sf::Sprite m_sprite;
    bool& m_isGameOver;

  
};
#endif  // __BOMB_H__