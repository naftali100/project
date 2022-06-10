#include "Game/Jail.h"

#include "Colors.h"

Jail::Jail(const LevelParams& p): m_bombBuffer(p.m_bombToScore){
    setCollisionTag(CollisionTag::jail);
    float scale = .9f;
    m_sprite.scale(sf::Vector2f(1,1) * scale);
    m_sprite.setTextureRect({0, 0, 500, 250});

    Entity::setSize({m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height});

    m_subs.push_back(MessageBus::subscribe<LevelParams*>(MessageType::LevelParamsUpdated, [this](LevelParams* i){
        m_bombBuffer = i->m_bombToScore;
    }));
    m_subs.push_back(MessageBus::subscribe(MessageType::FreeTerroristsGift, [this](){
        for(auto i: m_bombs){
            i->release();
        }
    }));
}


void Jail::update(const sf::Time& dt) {
    m_jailBreakTimer.update(dt);
};

void Jail::freeAll()
{
    m_isJailBreak = true;
    m_sprite.setTextureRect({ 0, 250, 500, 500 });
    m_jailBreakTimer.set([this]() {
        m_isJailBreak = false; 
        m_sprite.setTextureRect({ 0, 0, 500, 250 });
        }, 5.f);
    for (auto& terrorist : m_bombs)
        terrorist->release();
}

bool Jail::isBroken() const {
    return m_isJailBreak;
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
    for(auto const& i: m_subs){
        i();
    }   
}
