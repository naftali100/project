#include "Game/MovingObjects.h"

#include <math.h>

#include "Colors.h"
#include "Resources.h"

void MovingObjects::handleCollision(Entity* e, const sf::Vector3f& manifold) {
    switch (e->getCollisionTag()) {
        case CollisionTag::wall:
        case CollisionTag::jail:
            resolveCollision(manifold);
        default:
            break;
    }
}

// from: https://gist.github.com/fallahn/f81d23137409313e7de6
void MovingObjects::resolveCollision(const sf::Vector3f& manifold) {
    // move the ball out of the solid object by the penetration amount
    sf::Vector2f normal(manifold.x, manifold.y);
    move(normal * manifold.z);

    // reflect the current velocity to make the ball bounce
    m_direction = sf::util::reflect(m_direction, normal);
}

void MovingObjects::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(m_sprite, states);

    sf::String timerTitle;
    // only 2 places after the dot
    auto f = std::round(m_timer.asSeconds() * 100) / 100;
    timerTitle += std::to_string(f);

    sf::Text timer;
    timer.setFont(FontHolder::get(Fonts::Test));
    timer.setString(timerTitle);
    timer.setFillColor(Colors::White);
    timer.setOutlineColor(Colors::Black);
    timer.setOutlineThickness(2);

    auto topCenter = sf::util::getGlobalTopCenter(*this);

    timer.setPosition(topCenter.x - sf::util::getGlobalCenter(timer).x, topCenter.y - 20);
    target.draw(timer);

    sf::RectangleShape bound{getSize()};
    bound.setPosition(getGlobalBounds().left, getGlobalBounds().top);
    bound.setFillColor(Colors::Transparent);
    bound.setOutlineColor(Colors::Red);
    bound.setOutlineThickness(2);

    target.draw(bound);
}

void MovingObjects::update(const sf::Time& dt) {
    // timer update moved to childrens, because its need to be independent on movement
    
    if (m_timer.asSeconds() < 3)
        flicker();

    move(m_direction * m_speed * dt.asSeconds());
}