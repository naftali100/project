#include "Game/Door.h"

Door::Door(const sf::Vector2f& pos) {
    setPosition(pos);
    sf::Vector2u textureSize = m_sprite.getTexture()->getSize();
    m_animation.initFramesWithFixedSize(textureSize, 1, 5, 0.1f);
    m_animation.setFrame(0);
    setOrigin(m_sprite.getGlobalBounds().width/2, m_sprite.getGlobalBounds().height/2);
}

void Door::update(const sf::Time& dt) {
    m_animation.update(dt);
    m_sprite.setPosition(getPosition());
    // TODO: position door (maybe should be in game state)
    // m_sprite.move(m_sprite.getPosition() - sf::util::getGlobalCenter(m_sprite));
};

void Door::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // states.transform *= getTransform();
    // target.draw(m_sprite);
    Entity::draw(target);
}
