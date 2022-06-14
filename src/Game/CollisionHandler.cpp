#include "CollisionHandler.h"

void CollisionHandler::addCollision(std::string& e1, std::string& e2, CollisionHandlerFunction function)
{
    m_map.emplace(std::pair<std::string, std::string>{e1, e2}, function);
    if (e1 != e2)
        m_map.emplace(std::pair<std::string, std::string>{e2, e1}, function);
}

void CollisionHandler::runCollisionHandler(const Entity& e1, const Entity& e2) {
    if (m_map.find({ e1.getType(), e2.getType() }) != m_map.end()) {
        m_map.at({ e1.getType(), e2.getType() })(e1, e2);
    }
    if (m_map.find({ e2.getType(), e1.getType() }) != m_map.end()) {
        m_map.at({ e2.getType(), e1.getType() })(e1, e2);
    }
}