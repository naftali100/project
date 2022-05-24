#ifndef __WALL_H__
#define __WALL_H__
#pragma once

#include "Entity.h"

class Wall : public Entity {
public:
    Wall() {
        setCollisionTag(CollisionTag::wall);
    }
    virtual void update(const sf::Time& dt) override{};
    virtual void handleEvent(const sf::Event& e){
        // check in events map
    };
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override {
        // debug draw
        // white rectangle of global bound
        sf::RectangleShape r{getSize()};
        r.setPosition(getPosition());
        target.draw(r);
    }

private:
};

#endif  // __WALL_H__