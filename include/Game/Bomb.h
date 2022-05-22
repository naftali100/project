#pragma once

#include "Game/MovingObjects.h"

class Bomb : public MovingObjects {
public:
    Bomb(){
        setCollisionTag(bomb);
    }

    virtual bool isTimeout() override{
        return false;
    };

    virtual void update(const sf::Time& dt) override{
        // drag
    };
    virtual void handleEvent(const sf::Event& e) override{};
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {}



private:

};