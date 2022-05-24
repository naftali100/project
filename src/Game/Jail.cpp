#include "Game/Jail.h"

#include "Colors.h"

Jail::Jail() {
    setCollisionTag(CollisionTag::jail);
    m_rec.setSize({150, 150});
    setSize(m_rec.getSize());
}

void Jail::setColor(const sf::Color& c) {
    m_color = c;
    m_rec.setFillColor(c);
}

void Jail::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(m_rec, states);
}

sf::FloatRect Jail::getGlobalBounds() const {
    return getTransform().transformRect(m_rec.getGlobalBounds());
}
