#include "Game/Jail.h"

#include "Colors.h"

Jail::Jail(const LevelParams& p)
// :Animateable::Animateable(2, 1, Textures::Jail, 3.f)
{
    m_bombBuffer = p.m_bombToScore;
    setCollisionTag(CollisionTag::jail);
    m_sprite.scale(0.5, 0.5);

    Entity::setSize({m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height});

    m_sub = MessageBus::subscribe<LevelParams*>(MessageType::LevelParamsUpdated, [this](LevelParams* i){
        m_bombBuffer = i->m_bombToScore;
    });
}

void Jail::setColor(const sf::Color& c) {
    m_color = c;
    m_sprite.setColor(c);
}

sf::Color Jail::getColor() const {
    return m_color;
}

void Jail::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(m_sprite, states);

    Entity::draw(target);
}

sf::FloatRect Jail::getGlobalBounds() const {
    return getTransform().transformRect(m_sprite.getGlobalBounds());
}

void Jail::addBomb(Bomb* b) {
    m_bombs.push_back(b);
    if (m_bombs.size() >= m_bombBuffer) {
        for (auto i : m_bombs) { MessageBus::notify<Bomb*>(MessageType::BombRemoveFromVector, i); }
        m_bombs.clear();
    }
}


Jail::~Jail(){
    m_sub();
}
