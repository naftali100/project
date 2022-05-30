#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Resources.h"

class Animateable : public Entity {
public:
    Animateable(const int textureRows,
                const int textureCols,
                const enum Textures::ID texture,
                const float timeForFrame,
                const sf::Vector2f position = sf::Vector2f{0.0, 0.0}) {
        m_sprite.setTexture(TextureHolder::get(texture));
        sf::Vector2u textureSize = m_sprite.getTexture()->getSize();
        //int textureRows = 2;
        //int textureCols = 4;
        //float timeForFrame = .08;

        m_animation.initFramesWithFixedSize(textureSize, textureRows, textureCols, timeForFrame);
        m_animation.setDuration(7 * timeForFrame);      // ASK: what is 7 for?
        m_sprite.setPosition(position);
    }

    //~Animateable() = 0;

    sf::FloatRect getGlobalBounds() const override {
        return m_sprite.getGlobalBounds();
    }

    void update(const sf::Time& dt) override {
        m_animation.update(dt);
    };

    void handleEvent(const sf::Event& e) override {
        // no event to handle
    };

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override {
        target.draw(m_sprite);
    }

protected:
    sf::Sprite m_sprite;
    Animation m_animation{ m_sprite };

private:

};