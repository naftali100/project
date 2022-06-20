#include "ParticleSystem.h"

#include <sstream>
#include "Random.h"

ParticleSystem::ParticleSystem(const ParticleSystem& other){
    m_dissolutionRate = other.m_dissolutionRate;
    m_position = other.m_position;
    m_gravity = other.m_gravity;
    m_particleSpeed = other.m_particleSpeed;
    m_dissolutionRate;
    m_shape = other.m_shape;
}

void ParticleSystem::addParticles(const DerivedFromParticle auto& particle, int particlesAmount) {
    addParticles(std::make_unique<Particle>(particle), particlesAmount);
}

void ParticleSystem::addParticles(std::unique_ptr<Particle> particle, int particlesAmount){
    particle->m_pos.x = m_position.x;
    particle->m_pos.y = m_position.y;

    particle->rotate(Random::rnd<float>(0.0f, MAX_ROTATE));
    float angle;
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
        return;
    }
    particle->opacity = OPACITY;

    particle->init();
    m_particles.push_back(std::move(particle));
}


void ParticleSystem::update(const sf::Time& dt) {
    float time = dt.asSeconds();

    for (ParticleIterator it = m_particles.begin(); it != m_particles.end(); it++) {
        (*it)->m_vel.x += (*it)->m_gravity.x * time;
        (*it)->m_vel.y += (*it)->m_gravity.y * time;

        (*it)->m_pos.x += (*it)->m_vel.x * time * m_particleSpeed;
        (*it)->m_pos.y += (*it)->m_vel.y * time * m_particleSpeed;

        (*it)->opacity -= m_dissolutionRate;

        (*it)->update(sf::seconds(time)); // should atleast set new position (m_pos)

        // if ((*it)->m_pos.x > m_texture.getSize().x || (*it)->m_pos.x < 0 || (*it)->m_pos.y > m_texture.getSize().y ||
        //     (*it)->m_pos.y < 0 || 
        if((*it)->opacity < 10) {
            it = m_particles.erase(it);
            if (it == m_particles.end())
                return;
        }
    }
}

void ParticleSystem::draw(sf::RenderTarget& target) const {
    for(auto const& i: m_particles){
        i->draw(target);
    }
}

std::string ParticleSystem::getNumberOfParticlesString() {
    std::ostringstream oss;
    oss << m_particles.size();
    return oss.str();
}