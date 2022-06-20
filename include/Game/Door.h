#pragma once

#include "Animation.h"
#include "Game/Entity.h"
#include "Log.h"
#include "Resources.h"

class Door : public Entity {
public:
    Door(const sf::Vector2f& pos);
    void update(const sf::Time&) override {};

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

    // close when gift
    void close(const sf::Time& seconds);

    sf::FloatRect getGlobalBounds() const override {
        return m_sprite.getGlobalBounds();
    }

private:
    bool m_isOpen = false;
    sf::Sprite m_sprite{TextureHolder::get(Textures::Door)};
    Animation m_animation{m_sprite};
    // timer for closed time
};