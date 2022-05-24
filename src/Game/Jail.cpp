#include "Game/Jail.h"

Jail::Jail() {
    setCollisionTag(jail);
    m_rec.setSize({150, 150});
}

void Jail::setColor(const sf::Color& c) {
    m_color = c;
    m_rec.setFillColor(c);
}

sf::FloatRect Jail::getGlobalBounds() const {
    return m_rec.getGlobalBounds();
}

void Jail::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(m_rec, states);
}