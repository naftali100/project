#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include "SfmlUtil.h"

struct Particle {
    sf::Vector2f m_pos;  // Position
    sf::Vector2f m_vel;  // Velocity
    float opacity;
    // after position and velocity is set this is called to update internal particle etc
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void draw(sf::RenderTarget& r) const = 0;
    virtual ~Particle() = default;
};

typedef std::vector<Particle*>::iterator ParticleIterator;

#endif