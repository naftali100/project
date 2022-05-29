#pragma once
#ifndef __COLLISIONHANDLER_H__
#define __COLLISIONHANDLER_H__

#include <typeindex>

#include "Game/Entity.h"

class CollisionHandler {
public:
    using CollisionHandlerFunction = std::function<void(Entity&, Entity&)>;
    void addCollision(std::string& e1, std::string& e2, CollisionHandlerFunction function) {
        m_map.emplace(std::pair<std::string, std::string>{e1, e2}, function);
        if(e1 != e2)
            m_map.emplace(std::pair<std::string, std::string>{e2, e1}, function);
    }

    // TODO: maybe this function should also trigger the handler function
    // so when calling it you don't have to do: f = getCollisionHandler(m, n); if(f != nullptr) (*f)(m,n)
    CollisionHandlerFunction* getCollisionHandler(const Entity& e1, const Entity& e2) {
        if (m_map.find({e1.getType(), e2.getType()}) != m_map.end()) {
            return &m_map.at({e1.getType(), e2.getType()});
        }
        if (m_map.find({e2.getType(), e1.getType()}) != m_map.end()) {
            return &m_map.at({e2.getType(), e1.getType()});
        }
        return nullptr;
    }

private:
    std::map<std::pair<std::string, std::string>, CollisionHandlerFunction> m_map;
    // std::map<std::pair<std::type_info, std::type_info>, CollisionHandlerFunction> m_map;
};

#endif  // __COLLISIONHANDLER_H__