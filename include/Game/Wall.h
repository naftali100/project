#ifndef __WALL_H__
#define __WALL_H__
#pragma once

#include "Entity.h"

// basically a rectangle with collision tag set to wall
class Wall : public Entity {
public:
    Wall() {
        setCollisionTag(CollisionTag::wall);
    }
    virtual void update(const sf::Time& dt) override{};
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override {
        sf::RectangleShape r{getSize()};
        r.setPosition(getPosition());
        target.draw(r);
    }

private:
};

#endif  // __WALL_H__