#pragma once
#ifndef __BOMB_H__
#define __BOMB_H__

#include "Game/MovingObjects.h"
#include "Log.h"
#include "Resources.h"
#include "Explosion.h"

class Bomb : public MovingObjects {
public:
    explicit Bomb(std::vector<std::unique_ptr<Explosion>>& m_explosions, int& livesCounter);
    using MovingObjects::m_timer;

    void update(const sf::Time& dt) override;
    void handleEvent(const sf::Event& e) override; 
    void arrest() { m_isJailed = true; }
    bool isArrested() const { return m_isJailed; }

    void handleCollision(Entity* e, const sf::Vector3f& manifold) override;

private:
    bool m_first = false;
    bool m_isDragged = false;
    bool m_isJailed = false;
    sf::Color m_color;
    int& m_livesCounter;
    std::vector<std::unique_ptr<Explosion>>& m_explosions;
};
#endif  // __BOMB_H__
