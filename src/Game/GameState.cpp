#include "Game/States/GameState.h"

#include "Game/Bomb.h"
#include "Game/Door.h"
#include "Game/Gift.h"
#include "Game/Jail.h"
#include "Game/States/WelcomeState.h"
#include "Game/Wall.h"
#include "Random.h"
#include "Resources.h"
#include "SfmlUtil.h"
#include "StateManager.h"
#include "Game/States/LoseState.h"

void GameState::init() {
    initBackground();
    initDoors();
    initState();
    initWalls();
    initJail();
    registerMessageHandlers();
}

void GameState::initBackground() {
    m_background.setTexture(&TextureHolder::get(m_backgroundTexture));
    m_background.setSize((sf::Vector2f)getWinSize());
}

void GameState::initState() {
    m_bombSpawnTimer.setRandomInterval(
        [this]() {
            spawnBomb();
        },
        0, m_params.m_bombSpawnRate);
    m_giftSpawnTimer.setRandomInterval(
        [this]() {
            spawnGift();
        },
        m_params.m_giftSpawnRate / 2, m_params.m_giftSpawnRate);
    for (int _ : rng::views::iota(0, m_params.m_maxBomb)) {
        spawnBomb();
    }
}

void GameState::initJail() {
    auto winSize = getWinSize();
    auto jailSize = Jail(m_params, Colors::Red, {0, 0}).getSize();
    sf::Vector2f startPoint{jailSize.x / 2, winSize.y - (jailSize.y / 2.f)};

    for (auto i : std::views::iota(0, m_params.m_colors + 1)) {
        addJail({(i * jailSize.x) + startPoint.x + 10, startPoint.y}, Colors::STD_COLORS[i]);
    }
}

void GameState::addJail(const sf::Vector2f& pos, const sf::Color& color) {
    m_jails.push_back(std::make_unique<Jail>(m_params, color, pos));
}

void GameState::initWalls() {
    auto winSize = getWinSize();

    // left wall
    addWall({-100, -100}, {200, (float)winSize.y + 200});
    // bottom wall
    addWall({-100, (float)winSize.y - 100}, {(float)winSize.x + 200, 200});
    // right wall
    addWall({(float)winSize.x - 100, -100}, {200, (float)winSize.y + 200});
    // upper wall
    addWall({-100, -100}, {(float)winSize.x + 300, 200});
}

void GameState::addWall(const sf::Vector2f& pos, const sf::Vector2f& size){   
    m_static.push_back(std::make_unique<Wall>(pos, size));
}

void GameState::initDoors() {
    addDoor({110, 110});
    addDoor({500, 110});
    addDoor({1000, 110});
}

void GameState::addDoor(const sf::Vector2f& pos) {
    m_doors.push_back(std::make_unique<Door>(pos));
}

void GameState::handleEvent(const sf::Event& e) {
    for (auto const& item : m_moving) item->handleEvent(e);
}

void GameState::update(const sf::Time& dt) {
    LOGV;
    if (m_lives <= 0) {
        m_stateManager.replaceState(std::make_unique<LoseState>(m_stateManager));
        return;
    }
    m_sb.update(dt);
    if(m_nonJailedBomb == 1) spawnBomb();
    if (m_nonJailedBomb < m_params.m_maxBomb) m_bombSpawnTimer.update(dt);
    m_giftSpawnTimer.update(dt);

    for (auto const& i : m_doors) { i->update(dt); }
    for (auto const& i : m_moving) { i->update(dt); }
    for (auto const& i : m_jails) { i->update(dt); }
    for (auto const& i : m_static) { i->update(dt); }
    for (auto const& i : m_explosions) { i->update(dt); }

    handleCollisions(dt);

    std::erase_if(m_moving, [](const auto& item) { return item->isDead(); });
};

void GameState::draw(sf::RenderTarget& win) const {
    LOGV;
    win.draw(m_background);

    for (auto& m : m_moving) { m->draw(win); }
    for (auto& m : m_static) { m->draw(win); }
    for (auto& m : m_jails | std::views::take(m_params.m_colors + 1)) { m->draw(win); }
    for (auto& m : m_doors) { m->draw(win); }
    for (auto& m : m_explosions) { m->draw(win); }
    m_sb.draw(win);
    LOGV;
};

// from: https://gist.github.com/fallahn/f81d23137409313e7de6
sf::Vector3f GameState::getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal) const {
    // the collision normal is stored in x and y, with the penetration in z
    sf::Vector3f manifold;

    if (overlap.width < overlap.height) {
        manifold.x = (collisionNormal.x < 0) ? 1.f : -1.f;
        manifold.z = overlap.width;
    }
    else {
        manifold.y = (collisionNormal.y < 0) ? 1.f : -1.f;
        manifold.z = overlap.height;
    }

    return manifold;
}

void GameState::handleCollisions(const sf::Time&) {
    for (auto const& m : m_moving) {
        for (auto const& n : m_jails)
            if (!n->isBroken())
                processCollision(m, n);
        for (auto const& n : m_static) 
            processCollision(m, n);
    }
}

void GameState::processCollision(auto const& m, auto const& n) {
    sf::FloatRect overlap;
    if (m->getGlobalBounds().intersects(n->getGlobalBounds(), overlap)) {
        // method A.
        auto collisionNormal = n->getPosition() - m->getPosition();
        auto manifold = getManifold(overlap, collisionNormal);
        m->handleCollision(n.get(), manifold);
        // method B.
        m_col.runCollisionHandler(*m, *n);
    }
};

void GameState::spawnBomb() {
    m_moving.push_back(std::make_unique<Bomb>(
        m_explosions, 
        m_params,
        m_doors.at(Random::rnd(1, (int)m_doors.size()) - 1)->getPosition(),  // random door position
        sf::Vector2f{Random::rnd(-1.0f, 1.0f), Random::rnd(-1.0f, 1.0f)},
        getWinSize()));
    m_nonJailedBomb++;
}

void GameState::spawnGift() {
    m_moving.push_back(std::make_unique<Gift>(
        m_doors.at(Random::rnd(1, (int)m_doors.size()) - 1)->getPosition(),  // random door position
        sf::Vector2f{Random::rnd(-1.0f, 1.0f), Random::rnd(-1.0f, 1.0f)},     // random direction
    getWinSize()));
}

void GameState::registerMessageHandlers() {
    m_subscription.push_back(MessageBus::subscribe(MessageType::BombJailed, [this]() { 
        m_nonJailedBomb--; 
    }));
    m_subscription.push_back(MessageBus::subscribe(MessageType::BombTimedout, [this]() {
        m_lives--;
        m_nonJailedBomb--;
        if(m_lives > 0) m_explosionSound.play();
    }));
    m_subscription.push_back(MessageBus::subscribe<Bomb*>(MessageType::BombRemoveFromVector, [this](auto bomb) {
        bomb->kill();
        m_score++;
    }));
    m_subscription.push_back(MessageBus::subscribe(MessageType::ScoreGift, [this]() { m_score += 5; }));
    m_subscription.push_back(MessageBus::subscribe(MessageType::LiveGift, [this]() { m_lives++; }));
    m_subscription.push_back(MessageBus::subscribe(MessageType::RemoveTerroristsGift, [this]() {
        m_nonJailedBomb = 0;
        rng::for_each(m_moving, [](auto& i) {
            if (i->getCollisionTag() == CollisionTag::bomb)
                i->kill();
        });
    }));
    m_subscription.push_back(MessageBus::subscribe(MessageType::FreeTerroristsGift, [this]() { freeTerrorists(); }));
}

void GameState::freeTerrorists() const {
    for (auto const& i : m_jails) i->freeAll();
}

sf::Vector2u GameState::getWinSize() {
    return m_stateManager.getWin().getSize();
}

GameState::~GameState() {
    for (auto const& i : m_subscription) {
        i();
    }
}
