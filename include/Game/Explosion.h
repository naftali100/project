#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Resources.h"

class Explosion : public Entity {
public:
    Explosion() {
        m_sprite.setTexture(TextureHolder::get(Textures::Explosion));
        sf::Vector2u textureSize = m_sprite.getTexture()->getSize();
        int textureRows = 2;
        int textureCols = 4;

        float timeForFrame = .08;

        m_animation.initFramesWithFixedSize(textureSize, textureRows, textureCols, timeForFrame);
        m_animation.setDuration(7 * timeForFrame);
        m_sprite.setPosition(200, 200);
    }

    sf::FloatRect getGlobalBounds() const override {
        return m_sprite.getGlobalBounds();
    }

    void update(const sf::Time& dt) override {
        m_animation.update(dt);
    };

    void handleEvent(const sf::Event& e) override {
        // no event to handle
    };

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_sprite);
    }

private:
    sf::Sprite m_sprite;
    Animation m_animation{ m_sprite };
};