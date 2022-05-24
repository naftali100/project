#pragma once

#include "Game/Entity.h"

class Door : public Entity {
public:
    virtual void update(const sf::Time& dt) override{

    };

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override {}

    // close when gift
    void close(const sf::Time& seconds);

private:
    bool m_isOpen = false;
    // timer for closed time
};