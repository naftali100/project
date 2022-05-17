#include "ParticleSystem.h"

#include <sstream>

ParticleSystem::ParticleSystem(int width, int height) {
    m_transparent = sf::Color(0, 0, 0, 0);
    m_texture.create(width, height);
    m_sprite = sf::Sprite(m_texture.getTexture());
    m_position.x = 0.5f * width;
    m_position.y = 0.5f * height;
    m_particleSpeed = 20.0f;
    m_dissolve = false;
    m_dissolutionRate = 4;
    m_shape = Shape::CIRCLE;
}

ParticleSystem::~ParticleSystem() {
    for (ParticleIterator it = m_particles.begin(); it != m_particles.end(); it++) { delete *it; }
}

void ParticleSystem::update() {
    float time = m_clock.restart().asSeconds();

    for (ParticleIterator it = m_particles.begin(); it != m_particles.end(); it++) {
        (*it)->m_vel.x += m_gravity.x * time;
        (*it)->m_vel.y += m_gravity.y * time;

        (*it)->m_pos.x += (*it)->m_vel.x * time * m_particleSpeed;
        (*it)->m_pos.y += (*it)->m_vel.y * time * m_particleSpeed;

        if (m_dissolve)
            (*it)->opacity -= m_dissolutionRate;

        (*it)->update();

        if ((*it)->m_pos.x > m_texture.getSize().x || (*it)->m_pos.x < 0 || (*it)->m_pos.y > m_texture.getSize().y ||
            (*it)->m_pos.y < 0 || (*it)->opacity < 10) {
            delete (*it);
            it = m_particles.erase(it);
            if (it == m_particles.end())
                return;
        }
    }
}

void ParticleSystem::render() {
    m_texture.clear();
    for (ParticleIterator it = m_particles.begin(); it != m_particles.end(); it++) { (*it)->draw(m_texture); }
    m_texture.display();
}

std::string ParticleSystem::getNumberOfParticlesString() {
    std::ostringstream oss;
    oss << m_particles.size();
    return oss.str();
}