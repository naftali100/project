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

GameState::GameState(StateManager& sm, LevelParams LP)
    : State(sm), m_params(LP) {
    GameStateInitializer initializer(
        m_stateManager,
        m_moving,
        m_jails,
        m_static,
        m_explosions,
        m_doors,
        m_stars,
        m_starAnimation,
        m_cam,
        m_subscription,
        m_lives,
        m_score,
        m_nonJailedBomb,
        m_params,
        m_spawnTimer,
        m_sb,
        m_background);
    initializer.initBackground();
    initializer.initDoors();
    initializer.initState();
    initializer.initWalls();
    initializer.initJail();
    initializer.initCamera();
    initializer.registerMessageHandlers();
}

void GameState::init() {
    // initPlay();
    
}

void GameState::handleEvent(const sf::Event& e) {
    LOGV;
    m_cam.handleEvent(e);
    for (auto const& item : m_moving) item->handleEvent(e);
    LOGV;
}

void GameState::update(const sf::Time& dt) {
    LOGV;
    if (m_lives <= 0) {
        // m_stateManager.replaceState(std::make_unique<WelcomeState>(m_stateManager));
        // return;
    }
    //imGui();

    m_cam.update(dt);
    m_starAnimation.update(dt);
    if (m_nonJailedBomb < m_params.m_maxBomb) 
        m_spawnTimer.update(dt);
    
    for (auto const& i : m_doors) { i->update(dt); }
    for (auto const& i : m_moving) { i->update(dt); }
    for (auto const& i : m_jails) { i->update(dt); }
    for (auto const& i : m_static) { i->update(dt); }
    for (auto const& i : m_explosions) { i->update(dt); }

    handleCollisions(dt);

    std::erase_if(m_moving, [](const auto& item) { return item->isTimeout(); });

    LOGV;
};

//void GameState::imGui() {
//    static bool toggleDoors = true;
//    ImGui::Checkbox("toggle doors", &toggleDoors);
//    if (toggleDoors) {
//        if (m_doors.empty()) {
//            initDoors();
//        }
//    }
//    else {
//        if (!m_doors.empty()) {
//            m_doors.clear();
//        }
//    }
//    //TODO: hide before handing over
//    if (ImGui::Begin("level params", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
//        auto tmp = m_params;
//        ImGui::SliderInt("bomb speed", &m_params.m_speed, 100, 1000);
//        ImGui::SliderInt("spawn rate", &m_params.m_spawnRate, 1, 10);
//        ImGui::SliderInt("bomb timeout", &m_params.m_bombTime, 1, 10);
//        ImGui::SliderInt("color amount", &m_params.m_colors, 1, 3);
//        ImGui::SliderInt("non jailed at same time", &m_params.m_maxBomb, 5, 10);
//        ImGui::SliderInt("bomb to score", &m_params.m_bombToScore, 3, 10);
//        if (m_params != tmp) {
//            MessageBus::notify<LevelParams*>(MessageType::LevelParamsUpdated, &m_params);
//        }
//    }
//    ImGui::End();
//
//    if (ImGui::Button("add life")) {
//        m_lives++;
//    }
//    if (ImGui::Button("spawn bomb")) {
//        for (auto i = 0; i < 10; i++) spawnBomb();
//    }
//    if (ImGui::Button("delete all bombs")) {
//        m_nonJailedBomb = 0;
//        for (auto const& item : m_moving)
//            if (item->getCollisionTag() == CollisionTag::bomb)
//                item->kill();
//    }
//    if (ImGui::Button("spawn gift")) {
//        spawnGift();
//    }
//    if (ImGui::Button("reset view")) {
//        m_cam.resetView();
//    }
//
//    if (ImGui::Button("exit state")) {
//        m_stateManager.popState(); 
//        return;
//    }
//    if (ImGui::Button("exit program")) {
//        exit(EXIT_SUCCESS);
//        // m_stateManager.stop();
//        return;
//    }
//
//    ImGui::Text("bomb count: %lu", m_moving.size());
//}

void GameState::draw(sf::RenderTarget& win) const {
    LOGV;
    m_cam.draw(win);  // set view
    win.draw(m_background);

    auto localStars = m_stars;
    for (int i = 0; i < m_lives; i++) {
        localStars.setPosition(100 * (float)i, 0);
        win.draw(localStars);
    }
    m_sb.draw(win);

    for (auto& m : m_moving) { m->draw(win); }
    for (auto& m : m_static) { m->draw(win); }
    for (auto& m : m_jails | std::views::take(m_params.m_colors + 1)) { m->draw(win); }
    for (auto& m : m_doors) { m->draw(win); }
    for (auto& m : m_explosions) { m->draw(win); }
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
        for(auto const& n: m_static)
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

GameState::~GameState() {
    for (auto const& i : m_subscription) { 
        i(); 
    }
}
