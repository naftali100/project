#pragma once
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

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
    template<typename T>
    ParticleSystem(const sf::Vector2<T>& size): ParticleSystem((int)size.x, (int)size.y){};
    ~ParticleSystem();

// TODO: support screen resize (using view?)
// TODO: switch to vertexArray for better performance
    template<DerivedFromParticle P>
    void fuel(int particles);  // Adds new particles to m_particles
    void update();             // Updates position, velocity and opacity of all particles
    void draw(sf::RenderTarget&) const;             // Renders all particles onto m_image

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
    float m_dissolutionRate;
    unsigned char m_shape;

    std::vector<ParticlePtr> m_particles;
};

template<DerivedFromParticle T>
void ParticleSystem::fuel(int particles) {
    LOGV;
    float angle;
    for (int i = 0; i < particles; i++) {
        LOGV;
        ParticlePtr particle = std::make_unique<T>();
        particle->m_pos.x = m_position.x;
        particle->m_pos.y = m_position.y;

        particle->rotate(Random::rnd<float>(0.0f, 90.f));

        switch (m_shape) {
            case Shape::CIRCLE:

                angle = Random::rnd<float>(0.0f, 6.2832f);
                particle->m_vel.x = Random::rnd<float>(std::min(0.0f, cos(angle)), std::max(0.0f, cos(angle)));
                particle->m_vel.y = Random::rnd<float>(std::min(0.0f, sin(angle)), std::max(0.0f, sin(angle)));
                particle->m_gravity = m_gravity;
                break;
            case Shape::SQUARE:
                particle->m_vel.x = Random::rnd<float>(-1.0f, 1.0f);
                particle->m_vel.y = Random::rnd<float>(-1.0f, 1.0f);
                break;
            default:
                particle->m_vel.x = 0.5f;  // Easily detected
                particle->m_vel.y = 0.5f;  // Easily detected
        }

        if (particle->m_vel.x == 0.0f && particle->m_vel.y == 0.0f) {
            continue;
        }
        particle->opacity = 255;

        particle->init();
        m_particles.push_back(std::move(particle));
    }
    LOGV;
}

#endif