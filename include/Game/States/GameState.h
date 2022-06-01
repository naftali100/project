#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__
#pragma once

#include "Animation.h"
#include "CollisionHandler.h"
#include "Game/Door.h"
#include "Game/Explosion.h"
#include "Game/MovingObjects.h"
#include "MessageBus.h"
#include "State.h"
#include "StateComponents/Camera.h"

class GameState : public State {
public:
    using State::State;

    void init() override;
    void initLayout();
    void initJail();

    void handleEvent(const sf::Event&) override;
    void handleMessages();
    void update(const sf::Time& dt) override;
    void draw(sf::RenderTarget& win) const override;

    // spawn
    // TODO: make it templated
    void spawnBomb();
    void spawnGift();

private:
    void handleCollisions(const sf::Time&);
    sf::Vector3f getManifold(const sf::FloatRect& overlap, const sf::Vector2f& colNormal) const;

private:
    std::vector<std::unique_ptr<MovingObjects>> m_moving;
    std::vector<std::unique_ptr<Entity>> m_static;
    std::vector<std::unique_ptr<Explosion>> m_explosions;
    std::vector<Door> m_doors;
    CollisionHandler m_col;
    Camera m_cam;

    sf::Sprite m_stars;
    Animation m_starAnimation{m_stars};

    int m_lives = 3;
    int m_nonJailedBomb = 0;

    Timer m_spawnTimer;
};

#endif  // __GAMESTATE_H__