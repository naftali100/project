#include "Game/Gift.h"


#include "Resources.h"
#include "Random.h"

Gift::Gift() {
    setSpeed(200);
    m_sprite.setTexture(TextureHolder::get(Textures::Gift2));

    m_timer.set([this]() { kill(); }, 10);

    m_giftAnimation.initFramesWithFixedSize(m_sprite.getTexture()->getSize(), 3, 4, 0.1);
    m_giftAnimation.setFrame(11);

    Entity::setSize(sf::Vector2f(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height));
    setOrigin(Entity::getSize() / 2.f);
}

void Gift::update(const sf::Time& dt) {
    m_timer.update(dt);
    if(m_taken){
        m_giftAnimation.update(dt);
    }else{
        MovingObjects::update(dt);
    }
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

void Gift::takeGift() {
    if(!m_taken){
        m_taken = true;
        // MessageBus::notify(MessageType::ReleaseAllBombs);
        MessageBus::notify(getRandGift());
        // wait for animation
        m_timer.set([this]() { kill(); }, m_giftAnimation.getLength());
    }
}

const MessageType Gift::getRandGift() const {
    return MessageType(Random::rnd(int(MessageType::ScoreGift), int(MessageType::Count)));
}