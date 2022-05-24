#pragma once
#ifndef __BOMB_H__
#define __BOMB_H__

#include "Game/MovingObjects.h"
#include "Resources.h"
#include "Log.h"

class Bomb : public MovingObjects {
public:
    Bomb();

    virtual bool isTimeout() override{
        return false;
    };

    virtual void update(const sf::Time& dt) override{
        // drag
        // update movement

        move(m_direction * m_speed * dt.asSeconds());
    };

    sf::FloatRect getGlobalBounds() const {
        return getTransform().transformRect(m_sprite.getGlobalBounds());
    }

    void handleCollision(Entity* e) override;
    virtual void handleEvent(const sf::Event& e) override{};
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(m_sprite, states);
    }

private:
    sf::Sprite m_sprite;
};
#endif // __BOMB_H__