#pragma once
#include "Animation.h"
#include "Entity.h"
#include "Log.h"
#include "Resources.h"

class Animateable : public Entity {
public:
    Animateable(const int textureRows, const int textureCols, const Textures::ID texture, const float timeForFrame,
                const sf::Vector2f position = sf::Vector2f{0.0, 0.0}, const float duration = -1) {
        m_position = position;

        m_sprite.setTexture(TextureHolder::get(texture));

        sf::Vector2u textureSize = m_sprite.getTexture()->getSize();
        m_animation.initFramesWithFixedSize(textureSize, textureRows, textureCols, timeForFrame);

        if (duration == 0)
            // this value make the animation do only one one the entire animation sheet (amount of all frames)
            m_animation.setDuration(((textureRows * textureCols) - 1) * timeForFrame);
        else if (duration != -1)
            m_animation.setDuration(duration);
    }

    sf::FloatRect getGlobalBounds() const override {
        return m_sprite.getGlobalBounds();
    }

    void update(const sf::Time& dt) override {
        m_animation.update(dt);
        m_sprite.setPosition(m_position);
        m_sprite.move(m_sprite.getPosition() - sf::util::getGlobalCenter(m_sprite));
    };

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override {
        target.draw(m_sprite);
    }

    virtual ~Animateable() = default;

protected:
    sf::Vector2f m_position;
    sf::Sprite m_sprite;
    Animation m_animation{m_sprite};

private:
};