#ifndef __WALL_H__
#define __WALL_H__
#pragma once

#include "Entity.h"
#include "resource_holder.h"
#include "Resources.h"

//TODO: set texture
// basically a rectangle with collision tag set to wall
class Wall : public Entity {
public:
    Wall(const sf::Vector2f& pos, const sf::Vector2f& size, const float rotation = 0)
    : m_rotation(rotation) {
        setCollisionTag(CollisionTag::wall);
        setPosition(pos);
        setSize(size);
    }
    void update(const sf::Time& dt) override{};
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override {
        TextureHolder::get(Textures::Wall).setRepeated(true);
        sf::Sprite r;
        r.setTexture(TextureHolder::get(Textures::Wall));
        r.setOrigin({r.getGlobalBounds().width / 2, r.getGlobalBounds().height / 2 });
        r.setRotation(m_rotation);
        //sf::RectangleShape r{getSize()};
        //r.setTexture(&TextureHolder::get(Textures::Wall));
        auto size = getSize();
        r.setTextureRect({ 0,0, int(size.x), int(size.y)});
        //r.setTextureRect({ 0,0, int(size.x/3), int(size.y)});
        r.setPosition(getPosition());
        target.draw(r);
    }

private:
    int m_rotation;
};

#endif  // __WALL_H__