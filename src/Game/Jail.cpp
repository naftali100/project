#include "Game/Jail.h"

#include "Colors.h"

Jail::Jail(const LevelParams& p)
// :Animateable::Animateable(2, 1, Textures::Jail, 3.f)
{
    m_bombBuffer = p.m_bombToScore;
    setCollisionTag(CollisionTag::jail);
    float scale = 0.95;
    m_sprite.scale(sf::Vector2f(1,1) / scale);

    Entity::setSize({m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height});
    m_sprite.setTextureRect({0, 0, 500, 250});

    m_subs.push_back(MessageBus::subscribe<LevelParams*>(MessageType::LevelParamsUpdated, [this](LevelParams* i){
        m_bombBuffer = i->m_bombToScore;
    }));
    m_subs.push_back(MessageBus::subscribe(MessageType::FreeTerroristsGift, [this](){
        for(auto i: m_bombs){
            i->release();
        }
    }));
}

void Jail::freeAll()
{
    for (auto& terrorist : m_bombs)
        terrorist->release();
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
    for(auto i: m_subs){
        i();
    }   
}
