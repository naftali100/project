#ifndef __BOMB_H__
#define __BOMB_H__

#pragma once

#include "Game/MovingObjects.h"
#include "Resources.h"

class Bomb : public MovingObjects {
public:
    void func();
    Bomb();

    virtual bool isTimeout() override{
        return false;
    };

    virtual void update(const sf::Time& dt) override{
        // drag
        // update movement
        // setPosition();
    };
    virtual void handleEvent(const sf::Event& e) override{};
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        sf::Sprite s(TextureHolder::get(Textures::Bomb));

        states.transform *= getTransform();
        target.draw(s, states);
    }

private:

};
#endif // __BOMB_H__