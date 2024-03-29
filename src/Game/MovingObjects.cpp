#include "Game/MovingObjects.h"

#include <math.h>

#include "Colors.h"
#include "Resources.h"

#if defined(__linux__) || defined(__MINGW32__)
#include <iomanip>
#else
#include <format>
#endif

MovingObjects::MovingObjects(const sf::Vector2f& dir) {
    m_direction = sf::util::normalize(dir);
}

void MovingObjects::flicker() {
    // flickering obj
    m_sprite.setColor(Colors::STD_COLORS[m_flicker]);
    m_flicker++;
    m_flicker %= Colors::STD_COLORS.size();
}

void MovingObjects::setDirection(const sf::Vector2f& v) {
    m_direction = sf::util::normalize(v);
}

void MovingObjects::handleCollision(Entity* e, const sf::Vector3f& manifold) {
    switch (e->getCollisionTag()) {
        case CollisionTag::wall:
        case CollisionTag::jail:
            resolveCollision(manifold);
        default:;
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
    target.draw(m_sprite, getTransform());

    sf::String timerTitle;
    // only 2 places after the dot
    auto f = std::round(m_timer.asSeconds() * 100) / 100;
    std::string s;
// because linux not support yet the "format" library
#if defined(__linux__) || defined(__MINGW32__)
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << f;
    s = stream.str();
#else
    s = std::format("{:.2f}", f);
#endif
    timerTitle += s;

    auto topCenter = sf::util::getGlobalTopCenter(*this);

    sf::Text timer{timerTitle, FontHolder::get(Fonts::Test)};
    timer.setFillColor(Colors::White);
    timer.setOutlineColor(Colors::Black);
    timer.setOutlineThickness(2);
    timer.setPosition(topCenter.x - sf::util::getGlobalCenter(timer).x, topCenter.y - 20);
    target.draw(timer);

    Entity::draw(target);
}

void MovingObjects::update(const sf::Time& dt) {
    // timer update moved to childrens, because its need to be independent on movement

    if (m_timer.asSeconds() < 3)
        flicker();
    
    move(m_direction * m_speed * dt.asSeconds());
}