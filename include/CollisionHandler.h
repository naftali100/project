#pragma once
#ifndef __COLLISIONHANDLER_H__
#define __COLLISIONHANDLER_H__

#include <typeindex>

#include "Game/Entity.h"

class CollisionHandler {
public:
    using CollisionHandlerFunction = std::function<void(const Entity&, const Entity&)>;

    void addCollision(std::string& e1, std::string& e2, CollisionHandlerFunction function);
    void runCollisionHandler(const Entity& e1, const Entity& e2);

private:
    std::map<std::pair<std::string, std::string>, CollisionHandlerFunction> m_map;
};

#endif  // __COLLISIONHANDLER_H__