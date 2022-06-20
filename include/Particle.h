#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include "Log.h"
#include "Resources.h"

struct Particle {
    sf::Vector2f m_pos;  // Position
    sf::Vector2f m_vel;  // Velocity
    sf::Vector2f m_gravity;
    float opacity;
    // after position and velocity is set this is called to update internal particle etc
    virtual void init() = 0;
    virtual void update(const sf::Time& = {}) = 0;
    virtual void draw(sf::RenderTarget& r) const = 0;
    virtual void rotate(float) = 0;
    virtual ~Particle() = default;
};
using ParticlePtr = std::unique_ptr<Particle>;
using ParticleIterator = std::vector<ParticlePtr>::iterator;

struct BaseSpriteParticle : public Particle {
    sf::Sprite sprite{TextureHolder::get(Textures::Smoke)};

    void init() override {
        // float scale = 0.5;
        // sprite.scale(sf::Vector2f(1,1) * scale);
        sprite.setPosition(m_pos);
        sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
    }

    virtual void update(const sf::Time& dt) override {
        // update position and opacity
        sprite.setPosition(m_pos);
        sprite.setColor(sf::Color(255, 255, 255, opacity));
    }

    void draw(sf::RenderTarget& r) const override {
        r.draw(sprite);
    }

    void rotate(float angle) {
        sprite.rotate(angle);
    }
};

struct spriteParticle : public BaseSpriteParticle {

};

struct TrailParticle : public BaseSpriteParticle {
    sf::Color m_color;
    virtual void update(const sf::Time& dt) override {
        m_color.a = opacity;
        sprite.setPosition(m_pos);
        sprite.setColor(m_color);
        sprite.setScale(sf::Vector2f(1,1) * 1.5f * dt.asSeconds());
    }
};

#endif