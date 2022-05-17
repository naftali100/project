#pragma once
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include "Particle.h"
#include "Random.h"

template<typename T>
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
    ParticleSystem(int width, int height);
    ~ParticleSystem();

// TODO: support screen resize (using view?)
// TODO: switch to vertexArray for better performance
    template<DerivedFromParticle P>
    void fuel(int particles);  // Adds new particles to m_particles
    void update();             // Updates position, velocity and opacity of all particles
    void render();             // Renders all particles onto m_image

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
    void setDissolve(bool enable) {
        m_dissolve = enable;
    }
    void setDissolutionRate(unsigned char rate) {
        m_dissolutionRate = rate;
    }
    void setShape(unsigned char shape) {
        m_shape = shape;
    }

    int getNumberOfParticles() {
        return m_particles.size();
    }
    std::string getNumberOfParticlesString();
    sf::Sprite& getSprite() {
        return m_sprite;
    }

private:
    sf::Vector2f m_position;  // Particle origin (pixel co-ordinates)
    sf::Vector2f m_gravity;   // Affects particle velocities
    sf::Clock m_clock;        // Used to scale particle motion
    sf::Color m_transparent;  // sf::Color( 0, 0, 0, 0 )
    sf::RenderTexture m_texture;
    sf::Sprite m_sprite;    // Connected to m_image
    float m_particleSpeed;  // Pixels per second (at most)
    bool m_dissolve;        // Dissolution enabled?
    unsigned char m_dissolutionRate;
    unsigned char m_shape;

    std::vector<Particle*> m_particles;
};

template<DerivedFromParticle T>
void ParticleSystem::fuel(int particles) {
    float angle;
    T* particle;
    for (int i = 0; i < particles; i++) {
        particle = new T();
        particle->m_pos.x = m_position.x;
        particle->m_pos.y = m_position.y;

        switch (m_shape) {
            case Shape::CIRCLE:
                angle = Random::rnd(0.0f, 6.2832f);
                particle->m_vel.x = Random::rnd(0.0f, cos(angle));
                particle->m_vel.y = Random::rnd(0.0f, sin(angle));
                break;
            case Shape::SQUARE:
                particle->m_vel.x = Random::rnd(-1.0f, 1.0f);
                particle->m_vel.y = Random::rnd(-1.0f, 1.0f);
                break;
            default:
                particle->m_vel.x = 0.5f;  // Easily detected
                particle->m_vel.y = 0.5f;  // Easily detected
        }

        if (particle->m_vel.x == 0.0f && particle->m_vel.y == 0.0f) {
            delete particle;
            continue;
        }
        particle->opacity = 255;

        particle->init();
        m_particles.push_back(particle);
    }
}

#endif