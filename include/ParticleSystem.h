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

    // for coping all properties without the particles
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

    template <DerivedFromParticle P>
    void addParticles(int particles);            // Adds new particles to m_particles
    void addParticles(DerivedFromParticle auto particle, int particlesAmount);
    void addParticles(std::unique_ptr<Particle> particle, int particlesAmount);
    void update(const sf::Time&);        // Updates position, velocity and opacity of all particles
    void draw(sf::RenderTarget&) const;  // Renders all particles onto m_image

    void setPosition(float x, float y) {
        setPosition({x, y});
    }
    void setPosition(const sf::Vector2f& pos){
        m_position = pos;
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

void ParticleSystem::addParticles(DerivedFromParticle auto particle, int particlesAmount) {
    addParticles(std::make_unique<decltype(particle)>(particle), particlesAmount);
}

template <DerivedFromParticle T>
void ParticleSystem::addParticles(int particlesAmount) {
    for (int i = 0; i < particlesAmount; i++) {
        addParticles(std::make_unique<T>(), particlesAmount);
    }
}

#endif