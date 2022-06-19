#pragma once
#ifndef __BOMB_H__
#define __BOMB_H__

#include "Explosion.h"
#include "Game/LevelParams.h"
#include "Game/MovingObjects.h"
#include "Log.h"
#include "MessageBus.h"
#include "Resources.h"
#include "ParticleSystem.h"

class Bomb : public MovingObjects {
public:
    explicit Bomb(std::vector<std::unique_ptr<Explosion>>& m_explosions,
        const LevelParams& p,
        const sf::Vector2f& pos,
        const sf::Vector2f& dir,
        sf::Vector2u winSize);

    void configLevelParam(const LevelParams& p);
    void registerMessageHandler();
    void initSprite();
    void setSpriteByDirection();

    void update(const sf::Time& dt) override;
    void handleEvent(const sf::Event& e) override;
    void draw(sf::RenderTarget& win, sf::RenderStates states) const override;
    void arrest() {
        m_isJailed = true;
    }
    void release() {
        m_isJailed = false;
    }
    bool isArrested() const {
        return m_isJailed;
    }

    void handleCollision(Entity* e, const sf::Vector3f& manifold) override;

    ~Bomb() override;

private:
    bool m_isDragged = false;
    bool m_isJailed = false;
    sf::Color m_color;
    std::vector<std::unique_ptr<Explosion>>& m_explosions;

    Func m_sub;
    ParticleSystem m_footStep;
    int deffer = 5;
    sf::Vector2u m_winSize;
};

#endif  // __BOMB_H__
