#pragma once
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Particle.h"
#include "Random.h"

//constants:
const float PARTICLE_SPEED = 20.f;
const float MAX_ROTATE = 90.f;
const float PI = 3.1416f;
const float VEL = 0.5f;
const int OPACITY = 255;

template <typename T>
concept DerivedFromParticle = std::is_base_of<Particle, T>::value;

namespace Shape {
    enum
    {
        CIRCLE,
        SQUARE
    };
}  // namespace Shape

class ParticleSystem {
public:
    ParticleSystem() {
        m_particleSpeed = PARTICLE_SPEED;
        m_dissolutionRate = 0;
        m_shape = Shape::CIRCLE;
    }

    ParticleSystem(const ParticleSystem& other);

    ParticleSystem& operator=(const ParticleSystem& other) {
        if (this == &other)
            return *this;

        m_dissolutionRate = other.m_dissolutionRate;
        m_position = other.m_position;
        m_gravity = other.m_gravity;
        m_particleSpeed = other.m_particleSpeed;
        m_dissolutionRate;
        m_shape = other.m_shape;

        return *this;
    }

    // TODO: support screen resize (using view?)
    // TODO: switch to vertexArray for better performance
    template <DerivedFromParticle P>
    void fuel(int particles);            // Adds new particles to m_particles
    void update(const sf::Time&);        // Updates position, velocity and opacity of all particles
    void draw(sf::RenderTarget&) const;  // Renders all particles onto m_image

    void setPosition(float x, float y) {
        m_position.x = x;
        m_position.y = y;
    }
    void setGravity(float x, float y) {
        m_gravity.x = x;
        m_gravity.y = y;
    }
    void setParticleSpeed(float speed) {
        m_particleSpeed = speed;
    }
    void setDissolutionRate(float rate) {
        m_dissolutionRate = rate;
    }
    void setShape(unsigned char shape) {
        m_shape = shape;
    }

    int getNumberOfParticles() {
        return m_particles.size();
    }
    std::string getNumberOfParticlesString();

private:
    sf::Vector2f m_position;  // Particle origin (pixel co-ordinates)
    sf::Vector2f m_gravity;   // Affects particle velocities
    float m_particleSpeed;    // Pixels per second (at most)
    float m_dissolutionRate;
    unsigned char m_shape;

    std::vector<ParticlePtr> m_particles;
};

template <DerivedFromParticle T>
void ParticleSystem::fuel(int particles) {
    LOGV;
    float angle;
    for (int i = 0; i < particles; i++) {
        LOGV;
        ParticlePtr particle = std::make_unique<T>();
        particle->m_pos.x = m_position.x;
        particle->m_pos.y = m_position.y;

        particle->rotate(Random::rnd<float>(0.0f, MAX_ROTATE));

        switch (m_shape) {
            case Shape::CIRCLE:

                angle = Random::rnd<float>(0.0f, 2*PI);
                particle->m_vel.x = Random::rnd<float>(std::min(0.0f, std::cos(angle)), std::max(0.0f, std::cos(angle)));
                particle->m_vel.y = Random::rnd<float>(std::min(0.0f, std::sin(angle)), std::max(0.0f, std::sin(angle)));
                particle->m_gravity = m_gravity;
                break;
            case Shape::SQUARE:
                particle->m_vel.x = Random::rnd<float>(-1.0f, 1.0f);
                particle->m_vel.y = Random::rnd<float>(-1.0f, 1.0f);
                particle->m_gravity = m_gravity;
                break;
            default:
                particle->m_vel.x = VEL;  // Easily detected
                particle->m_vel.y = VEL;  // Easily detected
        }

        if (particle->m_vel.x == 0.0f && particle->m_vel.y == 0.0f) {
            continue;
        }
        particle->opacity = OPACITY;

        particle->init();
        m_particles.push_back(std::move(particle));
    }
    LOGV;
}

#endif