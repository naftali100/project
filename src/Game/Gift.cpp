#include "Game/Gift.h"

#include "Resources.h"

Gift::Gift() {
    setSpeed(200);
    m_sprite.setTexture(TextureHolder::get(Textures::Gift));

    m_timer.set([this]() { m_isTimeOut = true; }, 10);  // TODO: calc delay
    m_anim.initFramesWithFixedSize(TextureHolder::get(Textures::Gift).getSize(), 2, 2, 0.1);
    m_anim.setFrame(0);

    Entity::setSize(sf::Vector2f(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height));
    setOrigin(Entity::getSize() / 2.f);
}

void Gift::handleEvent(const sf::Event& e) {
    switch (e.type) {
        case sf::Event::MouseButtonReleased:
            if (getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                takeGift();
            }
            break;
        default:;
    }
}