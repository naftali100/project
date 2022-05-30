#include "Game/Jail.h"

#include "Colors.h"

Jail::Jail() 
:Animateable::Animateable(2, 1, Textures::ID::Jail, 3.f) 
{
    setCollisionTag(CollisionTag::jail);
    //m_rec.setSize({150, 150});
    m_sprite.scale(0.5, 0.5);

    //TODO: who is first here, hight or width?
    setSize({ m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height });
    
}

void Jail::setColor(const sf::Color& c) {
    m_color = c;
    //m_rec.setFillColor(c);
    m_sprite.setColor(c);
}

sf::Color Jail::getColor() const {
    return m_color;
}

void Jail::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(m_sprite, states);
}

sf::FloatRect Jail::getGlobalBounds() const {
    return getTransform().transformRect(m_sprite.getGlobalBounds());
}
