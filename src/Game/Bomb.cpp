#include "Game/Bomb.h"

#include <mutex>

#include "Game/Jail.h"
#include "SfmlUtil.h"
#include "MessageBus.h"

Bomb::Bomb(std::vector<std::unique_ptr<Explosion>>& explosions, int& livesCounter, int& noJailedCounter) : m_explosions(explosions), m_livesCounter(livesCounter), m_nonJailedBombCounter(noJailedCounter) {
    // for ImGui to print only once.
    // see update()
    static std::once_flag flag;
    std::call_once(flag, [this]() { m_first = true; });

    setCollisionTag(CollisionTag::bomb);
    setSpeed(300);
    m_sprite.setTexture(TextureHolder::get(Textures::Bomb));
    m_sprite.scale(0.25, 0.25);
    MovingObjects::setSize(sf::Vector2f(TextureHolder::get(Textures::Bomb).getSize() / 4u));
    setOrigin(MovingObjects::getSize() / 2.f);
    m_timer.set(
        [this]() {
            // MessageBus::postMessage(MessageType::BombTimedout, this);
            MessageBus::postMessage(MessageType::BombTimedout);
            m_isTimeOut = true;
            // m_livesCounter--;
            // m_nonJailedBombCounter--;
            m_explosions.push_back(std::make_unique<Explosion>(getPosition()));
        },
        10);  // TODO: calc delay

    m_color = Colors::White;
}

void Bomb::update(const sf::Time& dt) {
    static float speed = m_speed;
    if (m_first)
        ImGui::SliderFloat("bomb speed", &speed, 0, 1000);

    m_speed = speed;

    if (m_isJailed)
        return;

    // need to update timer event if dragged
    m_timer.update(dt);

    // update movement
    if (!m_isDragged)
        MovingObjects::update(dt);
};

void Bomb::handleEvent(const sf::Event& e) {
    // drag
    switch (e.type) {
        case sf::Event::MouseButtonPressed:
            if (getGlobalBounds().contains((float)e.mouseButton.x, (float)e.mouseButton.y)) {
                m_isDragged = true;
            }
            break;
        case sf::Event::MouseButtonReleased:
            m_isDragged = false;
            break;
        case sf::Event::MouseMoved:
            if (m_isDragged && !m_isJailed)
                setPosition((float)e.mouseMove.x, (float)e.mouseMove.y);
            break;
        default:;
    }
}

void Bomb::handleCollision(Entity* e, const sf::Vector3f& manifold) {
    if(m_isJailed) return;
    if (e->getCollisionTag() == CollisionTag::jail) {
        sf::FloatRect tempRect;
        // if intersects, and the whole entity is inside the jail
        if (getGlobalBounds().intersects(e->getGlobalBounds(), tempRect) && 
            tempRect.width == getGlobalBounds().width && tempRect.height == getGlobalBounds().height) {
            auto jail = dynamic_cast<Jail*>(e);  // needed for getting jail's color. TODO: can we avoid this?
            if (m_color != jail->getColor()) {
                // make the bomb to timeout and "explode"
                m_timer.reset();
            }
            else {
                MessageBus::postMessage(MessageType::BombJailed);
                // MessageBus::postMessage()
                jail->addBomb(this);
                arrest();
                // m_nonJailedBombCounter --;
            }
        }
    }

    if (!m_isDragged)
        MovingObjects::handleCollision(e, manifold);
}
