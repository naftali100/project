#ifndef __WALL_H__
#define __WALL_H__
#pragma once

#include "Entity.h"
#include "Resources.h"

//  basically a rectangle with collision tag set to wall
class Wall : public Entity {
public:
    Wall(const sf::Vector2f& pos, const sf::Vector2f& size) {
        setCollisionTag(CollisionTag::wall);
        setPosition(pos);
        setSize(size);
    }
    void update(const sf::Time& dt) override{};
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override {
        sf::Sprite r{TextureHolder::get(Textures::Wall), sf::IntRect((sf::Vector2i)getPosition(), (sf::Vector2i)getSize())};
        r.setPosition(getPosition());
        target.draw(r);
    }

private:
};

#endif  // __WALL_H__