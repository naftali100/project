#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__
#pragma once

#include "Animation.h"
#include "CollisionHandler.h"
#include "Game/Door.h"
#include "Game/Explosion.h"
#include "Game/Gift2.h"
#include "Game/LevelParams.h"
#include "Game/MovingObjects.h"
#include "Game/StatusBar.h"
#include "Game/Terrorist.h"
#include "MessageBus.h"
#include "State.h"
#include "StateComponents/Camera.h"
#include "Game/Jail.h"

class GameState : public State {
public:
    using State::State;

    void init() override;
    void initState();
    void initLayout();
    void initJail();
    void initDoors();
    void freeTerrorists();
    void processColision(auto const& m, auto const& n);
    void initCamera(); // TODO: delete

    void handleEvent(const sf::Event&) override;
    void update(const sf::Time& dt) override;
    void draw(sf::RenderTarget& win) const override;

    // spawn
    // TODO: make it templated
    void spawnBomb();
    void spawnGift();

    // util
    sf::Vector2u getWinSize();

    ~GameState();

private:
    void imGui(); // TODO: delete
    void registerMessageHandlers();
    void handleCollisions(const sf::Time&);
    sf::Vector3f getManifold(const sf::FloatRect& overlap, const sf::Vector2f& colNormal) const;

private:
    // objects
    std::vector<std::unique_ptr<MovingObjects>> m_moving;
    std::vector<std::unique_ptr<Jail>> m_jails;
    std::vector<std::unique_ptr<Entity>> m_static;
    std::vector<std::unique_ptr<Explosion>> m_explosions;
    std::vector<std::unique_ptr<Door>> m_doors;
    sf::Sprite m_stars;
    Animation m_starAnimation{m_stars};

    // components
    CollisionHandler m_col;
    Camera m_cam;
    std::vector<MessageBus::Func> m_subscription;

    // game data
    int m_lives = 3;
    int m_score = 0;
    int m_nonJailedBomb = 0;
    LevelParams m_params;

    Timer m_spawnTimer;
    StatusBar m_sb{m_lives, m_score};
};

#endif  // __GAMESTATE_H__