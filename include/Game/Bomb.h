#pragma once
#ifndef __BOMB_H__
#define __BOMB_H__

#include "Explosion.h"
#include "Game/LevelParams.h"
#include "Game/MovingObjects.h"
#include "Log.h"
#include "MessageBus.h"
#include "Resources.h"

class Bomb : public MovingObjects {
public:
    explicit Bomb(std::vector<std::unique_ptr<Explosion>>& m_explosions, const LevelParams& p);
    void initFromLevelParam(const LevelParams& p, bool = true);
    void registerMessageHandler();

    void update(const sf::Time& dt) override;
    void handleEvent(const sf::Event& e) override;
    void draw(sf::RenderTarget& win, sf::RenderStates states) const override;
    void arrest() { m_isJailed = true; }
    bool isArrested() const { return m_isJailed; }

    void handleCollision(Entity* e, const sf::Vector3f& manifold) override;

    ~Bomb() override;

private:
    bool m_isDragged = false;
    bool m_isJailed = false;
    sf::Color m_color;
    // int& m_livesCounter;
    // int& m_nonJailedBombCounter;
    std::vector<std::unique_ptr<Explosion>>& m_explosions;

    MessageBus::Func m_sub;

    Animation m_terroristAnimation{m_sprite};
};
#endif  // __BOMB_H__
