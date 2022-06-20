#include "Game/Door.h"

Door::Door(const sf::Vector2f& pos) {
    setPosition(pos);
    sf::Vector2u textureSize = m_sprite.getTexture()->getSize();
    setSize((sf::Vector2f)textureSize);
    setOrigin(getSize() / 2.f);
}

void Door::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_sprite, getTransform());
    Entity::draw(target);
}
