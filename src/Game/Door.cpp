#include "Game/Door.h"

Door::Door(){
    sf::Vector2u textureSize = m_sprite.getTexture()->getSize();
    m_animation.initFramesWithFixedSize(textureSize, 1, 5, 0.1f);
}

void Door::draw(sf::RenderTarget& target, sf::RenderStates states) const  {
    // states.transform *= getTransform();
    // target.draw(m_sprite);
    Entity::draw(target);
}

void Door::update(const sf::Time& dt) {
    plog::get()->setMaxSeverity(plog::debug);
    m_animation.update(dt);
    plog::get()->setMaxSeverity(plog::info);
    m_sprite.setPosition(getPosition());
    // TODO: position door (maybe should be in game state)
    // m_sprite.move(m_sprite.getPosition() - sf::util::getGlobalCenter(m_sprite));
};
