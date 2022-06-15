#pragma once

#include "Game/Entity.h"
#include "Resources.h"
#include "Animation.h"
#include "Log.h"

class Door : public Entity {
public:
    Door(const sf::Vector2f& pos);
    void update(const sf::Time& dt) override;

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

    // close when gift
    void close(const sf::Time& seconds);

    sf::FloatRect getGlobalBounds() const override { 
        return m_sprite.getGlobalBounds();
    }

private:
    bool m_isOpen = false;
    sf::Sprite m_sprite { TextureHolder::get(Textures::Door) };
    Animation m_animation { m_sprite };
    // timer for closed time
};