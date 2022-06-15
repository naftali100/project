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
#include "StateManager.h"
#include "Game/Jail.h"
#include "Random.h"
#include "Game/Wall.h"
#include "Game/Gift.h"

class GameStateInitializer {
public:
    //c-tor
    GameStateInitializer(StateManager& stateManager,
        std::vector<std::unique_ptr<MovingObjects>>& moving,
        std::vector<std::unique_ptr<Jail>>& jails,
        std::vector<std::unique_ptr<Entity>>& statics,
        std::vector<std::unique_ptr<Explosion>>& explosions,
        std::vector<std::unique_ptr<Door>>& doors,
        sf::Sprite& stars,
        Animation& starAnimation,
        // components
        //CollisionHandler m_col; // currently not in use
        Camera& cam,
        std::vector<Func>& subscription,
        // game data
        int& lives,
        int& score,
        int& nonJailedBomb,
        LevelParams& params,
        Timer& spawnTimer,
        StatusBar& sb,
        sf::RectangleShape& background);

    // functions
    void initState();
    void initBackground();
    void initWalls();
    void addWall(const sf::Vector2f& pos, const sf::Vector2f& size);
    void initJail();
    void addJail(const sf::Vector2f& pos, const sf::Color& color);
    void initDoors();
    void initCamera(); // TODO: delete
    void registerMessageHandlers();

    // spawn
    void spawnBomb();
    void spawnGift();



private:
    // util
    sf::Vector2u getWinSize();
    void freeTerrorists() const;

private:
    StateManager& m_stateManager;
    std::vector<std::unique_ptr<MovingObjects>>& m_moving;
    std::vector<std::unique_ptr<Jail>>& m_jails;
    std::vector<std::unique_ptr<Entity>>& m_static;
    std::vector<std::unique_ptr<Explosion>>& m_explosions;
    std::vector<std::unique_ptr<Door>>& m_doors;
    sf::Sprite& m_stars;
    Animation& m_starAnimation;

    // components
    //CollisionHandler m_col; // currently not in use
    Camera& m_cam;
    std::vector<Func>& m_subscription;

    // game data
    int& m_lives;
    int& m_score;
    int& m_nonJailedBomb;
    LevelParams& m_params;

    Timer& m_spawnTimer;
    StatusBar& m_sb;
    sf::RectangleShape& m_background;
};