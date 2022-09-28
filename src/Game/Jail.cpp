#include "Game/Jail.h"

#include "Colors.h"

Jail::Jail(const LevelParams& p, const sf::Color& color, const sf::Vector2f& pos) : m_terroristBuffer(p.m_terroristToScore) {
    setPosition(pos);
    setColor(color);
    setCollisionTag(CollisionTag::jail);
    float scale = .9f;
    m_sprite.scale(sf::Vector2f(1, 1) * scale);
    m_sprite.setTextureRect({0, 0, 500, 250});

    Entity::setSize({m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height});
    setOrigin(getSize() / 2.f);

    m_subs.push_back(MessageBus::subscribe(MessageType::FreeTerroristsGift, [this]() {
        for (auto i : m_terrorists) {
            i->release();
        }
    }));
}

void Jail::update(const sf::Time& dt) {
    // MovingObjects::update(dt);
    m_jailBreakTimer.update(dt);
    // for(auto& terrorist: m_terrorists)
    //     terrorist->move(m_direction * m_speed * dt.asSeconds());
};

void Jail::freeAll() {
    m_isJailBreak = true;
    m_sprite.setTextureRect({0, 250, 500, 500});
    m_jailBreakTimer.setTimeout(
        [this]() {
            // m_isJailBreak = false;
            m_sprite.setTextureRect({0, 0, 500, 250});
        },
        3.f);
    for (auto& terrorist : m_terrorists) terrorist->release();
}

bool Jail::isBroken() const {
    return false;
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

void Jail::addTerrorist(Terrorist* b) {
    m_terrorists.push_back(b);
    if (m_terrorists.size() >= m_terroristBuffer) {
        for (auto i : m_terrorists) {
            MessageBus::notify<Terrorist*>(MessageType::TerroristRemoveFromVector, i);
        }
        m_terrorists.clear();
    }
}

Jail::~Jail() {
    for (auto const& i : m_subs) {
        i();
    }
}
