#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__
#pragma once

#include "Animation.h"
#include "CollisionHandler.h"
#include "Game/Door.h"
#include "Game/Explosion.h"
#include "Game/LevelParams.h"
#include "Game/MovingObjects.h"
#include "Game/StatusBar.h"
#include "MessageBus.h"
#include "State.h"
#include "StateComponents/Camera.h"
#include "Game/Jail.h"

//constants:
const int DEFAULT_LIVES = 3;


class GameState : public State {
public:
    using State::State;
    GameState(StateManager& sm, LevelParams LP, Textures::ID texture) :
        State(sm), m_params(LP), m_backgroundTexture(texture) {};

    void init() override;
    
    void handleEvent(const sf::Event&) override;
    void update(const sf::Time& dt) override;
    void draw(sf::RenderTarget& win) const override;

    // spawn
    void spawnBomb();
    void spawnGift();

    // util
    sf::Vector2u getWinSize();

    ~GameState();

private:
    void initState();
    void initBackground();
    void initWalls();
    void addWall(const sf::Vector2f& pos, const sf::Vector2f& size);

    void initJail();
    void addJail(const sf::Vector2f& pos, const sf::Color& color);

    void initDoors();
    void addDoor(const sf::Vector2f& pos);

    void freeTerrorists() const;

    void registerMessageHandlers();

    void handleCollisions(const sf::Time&);
    void processCollision(auto const& m, auto const& n);
    sf::Vector3f getManifold(const sf::FloatRect& overlap, const sf::Vector2f& colNormal) const;

private:
    // objects
    std::vector<std::unique_ptr<MovingObjects>> m_moving;
    std::vector<std::unique_ptr<Jail>> m_jails;
    std::vector<std::unique_ptr<Entity>> m_static;
    std::vector<std::unique_ptr<Explosion>> m_explosions;
    std::vector<std::unique_ptr<Door>> m_doors;

    // components
    std::vector<Func> m_subscription;

    // game data
    int m_lives = DEFAULT_LIVES;
    int m_score = 0;
    int m_nonJailedBomb = 0;
    LevelParams m_params;

    Timer m_bombSpawnTimer;
    Timer m_giftSpawnTimer;
    StatusBar m_sb{m_lives, m_score};
    sf::RectangleShape m_background;
    sf::Sound m_explosionSound{SoundBufferHolder::get(SoundEffect::Explosion)};
    Textures::ID m_backgroundTexture = Textures::SkyBackground;
};

#endif  // __GAMESTATE_H__