#pragma once

#include "Game/Entity.h"
#include "Resources.h"

class Door : public Entity {
public:
    virtual void update(const sf::Time& dt) override{

    };

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override {
        states.transform *= getTransform();
        target.draw(m_sprite, states);
    }

    // close when gift
    void close(const sf::Time& seconds);

    sf::FloatRect getGlobalBounds() const override { 
        return m_sprite.getGlobalBounds();
    }

private:
    bool m_isOpen = false;
    sf::Sprite m_sprite { TextureHolder::get(Textures::Door) };
    // timer for closed time
};