#include "ParticleSystem.h"

#include <sstream>
#include "Random.h"

ParticleSystem::ParticleSystem(int width, int height) {
    m_position.x = 0.5f * width;
    m_position.y = 0.5f * height;
    m_particleSpeed = 20.0f;
    m_dissolutionRate = 0;
    m_shape = Shape::CIRCLE;
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