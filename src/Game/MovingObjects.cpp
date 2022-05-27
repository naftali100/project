#include "Game/MovingObjects.h"

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