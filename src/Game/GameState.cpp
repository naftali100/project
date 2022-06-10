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

void GameState::init() {
    initDoors();
    initState();
    initWalls();
    initJail();
    initCamera();
    // initPlay();
    registerMessageHandlers();
}

void GameState::initCamera(){
    m_cam.setView(m_stateManager.getWin().getDefaultView());
    m_cam.setInitialView();
    m_cam.setWinRatio(m_stateManager.getWin().getSize());
    m_cam.setResizeStrategy(LatterBox);
}

void GameState::initState() {
    m_stars.setTexture(TextureHolder::get(Textures::Stars));

    static float spawnInterval = 3;
    m_spawnTimer.set(
        [this]() {
            m_spawnTimer.setTime(sf::seconds(Random::rnd(0.f, m_params.m_spawnRate)));
            spawnBomb();
        },
        m_params.m_spawnRate);

    m_starAnimation.initFramesWithFixedSize(m_stars.getTexture()->getSize(), 1, 9, 0.1f);

    for (int _ : rng::views::iota(1, 10)){
        spawnBomb();
        // spawnGift();
    }
}

void GameState::initJail() {
    auto winSize = getWinSize();
    auto jailAmount = m_params.m_colors + 1;
    auto jailSize = Jail(m_params).getSize();
    // TODO: find the right position
    sf::Vector2f startPoint { (winSize.x - (jailSize.x * (float)jailAmount)) / 2.f, winSize.y - (jailSize.y / 2.f)};

    LOGI << PLOG_PRINT_VAR(winSize) << ' ' << PLOG_PRINT_VAR(startPoint);
    for(auto i: std::views::iota(0,jailAmount)){
        LOGI << i;
        addJail({((i+1) * jailSize.x) + startPoint.x + 10, startPoint.y}, Colors::STD_COLORS[i]);
    }
}

void GameState::addJail(const sf::Vector2f& pos, const sf::Color& color){
    auto j = std::make_unique<Jail>(m_params);
    j->setColor(color);
    j->setOrigin(sf::util::getGlobalCenter(*j));
    j->setPosition(pos);
    m_jails.push_back(std::move(j));
}

void GameState::initWalls() {
    auto winSize = getWinSize();

    // left wall
    addWall({-100, -100}, {100, (float)winSize.y + 200});
    // bottom wall
    addWall({-100, (float)winSize.y}, {(float)winSize.x + 200, 100});
    // right wall
    addWall({(float)winSize.x, -100}, {100, (float)winSize.y + 200});
    // upper wall
    addWall({-100, -100}, {(float)winSize.x + 200, 100});
}


void GameState::addWall(const sf::Vector2f& pos, const sf::Vector2f& size){   
    auto wall = std::make_unique<Wall>();
    wall->setPosition(pos);
    wall->setSize(size);
    m_static.push_back(std::move(wall));
}

void GameState::initDoors() {
    auto winSize = getWinSize();

    m_doors.clear();

    // doors in corners for now.
    auto d = std::make_unique<Door>();
    m_doors.push_back(std::move(d));
    m_doors.back()->setPosition(0, 0);
    d = std::make_unique<Door>();
    m_doors.push_back(std::move(d));
    m_doors.back()->setPosition(10, (float)winSize.y - 10);
    d = std::make_unique<Door>();
    m_doors.push_back(std::move(d));
    m_doors.back()->setPosition((float)winSize.x - 10, (float)winSize.y - 10);
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
    imGui();

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

void GameState::imGui() {
    static bool toggleDoors = true;
    ImGui::Checkbox("toggle doors", &toggleDoors);
    if (toggleDoors) {
        if (m_doors.empty()) {
            initDoors();
        }
    }
    else {
        if (!m_doors.empty()) {
            m_doors.clear();
        }
    }

    if (ImGui::Begin("level params", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
        auto tmp = m_params;
        ImGui::SliderFloat("bomb speed", &m_params.m_speed, 100, 1000);
        ImGui::SliderFloat("spawn rate", &m_params.m_spawnRate, 1, 10);
        ImGui::SliderInt("color amount", &m_params.m_colors, 1, 3);
        ImGui::SliderInt("non jailed at same time", &m_params.m_maxBomb, 5, 10);
        ImGui::SliderInt("bomb to score", &m_params.m_bombToScore, 3, 10);
        if (m_params != tmp) {
            MessageBus::notify<LevelParams*>(MessageType::LevelParamsUpdated, &m_params);
        }
    }
    ImGui::End();

    if (ImGui::Button("add life")) {
        m_lives++;
    }
    if (ImGui::Button("spawn bomb")) {
        for (auto i = 0; i < 10; i++) spawnBomb();
    }
    if (ImGui::Button("delete all bombs")) {
        m_nonJailedBomb = 0;
        m_moving.clear();
    }
    if (ImGui::Button("spawn gift")) {
        spawnGift();
    }
    if (ImGui::Button("reset view")) {
        m_cam.resetView();
    }

    if (ImGui::Button("exit state")) {
        // will cause seg fault because the update method is continuing after the state has been poped
        m_stateManager.popState(); 
        return;
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("will cause seg fault\nbecause the update method is continuing after the state has been poped");
    }
    if (ImGui::Button("exit program")) {
        exit(EXIT_SUCCESS);
        // m_stateManager.stop();
        return;
    }

    ImGui::Text("bomb count: %lu", m_moving.size());
}

void GameState::draw(sf::RenderTarget& win) const {
    LOGV;
    m_cam.draw(win);  // set view

    auto localStars = m_stars;
    for (int i = 0; i < m_lives; i++) {
        localStars.setPosition(100 * (float)i, 0);
        win.draw(localStars);
    }
    m_sb.draw(win);

    for (auto& m : m_moving) { m->draw(win); }
    for (auto& m : m_static) { m->draw(win); }
    for (auto& m : m_jails) { m->draw(win, sf::RenderStates::Default); }
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

void GameState::spawnBomb() {
    auto winSize = getWinSize();
    auto b = std::make_unique<Bomb>(m_explosions, m_params);
    b->setDirection({static_cast<float>(Random::rnd(-1.0, 1.0)), static_cast<float>(Random::rnd(-1.0, 1.0))});
    if (!m_doors.empty())
        b->setPosition(m_doors.at(Random::rnd(1, (int)m_doors.size()) - 1)->getPosition());
    else
        b->setPosition((float)Random::rnd(10, winSize.x - 10), (float)Random::rnd(10, winSize.y - 10));
    m_moving.push_back(std::move(b));
    m_nonJailedBomb++;
}

void GameState::spawnGift() {
    auto winSize = getWinSize();
    auto b = std::make_unique<Gift>();
    b->setDirection({static_cast<float>(Random::rnd(1.0, 100.0)), static_cast<float>(Random::rnd(1.0, 100.0))});
    b->setPosition((float)Random::rnd(10, winSize.x - 10), (float)Random::rnd(10, winSize.y - 10));
    m_moving.push_back(std::move(b));
}

void GameState::registerMessageHandlers() {
    m_subscription.push_back(
        MessageBus::subscribe(MessageType::BombJailed, [this]() {
            m_nonJailedBomb--;
        })
    );
    m_subscription.push_back(
        MessageBus::subscribe(MessageType::BombTimedout, [this]() {
            m_lives--;
            m_nonJailedBomb--;
    }));
    m_subscription.push_back(
        MessageBus::subscribe<Bomb*>(MessageType::BombRemoveFromVector, [this](auto bomb) {
            bomb->kill();
            m_score++;
    }));
    m_subscription.push_back(
        MessageBus::subscribe(MessageType::ScoreGift, [this](){
            m_score+=5;
        })
    );
    m_subscription.push_back(
        MessageBus::subscribe(MessageType::LiveGift, [this](){
            m_lives++;
        })
    );
    m_subscription.push_back(
        MessageBus::subscribe(MessageType::RemoveTerroristsGift, [this](){
            // m_nonJailedBomb = 0;
            // m_moving.clear();
        })
    );
    m_subscription.push_back(
        MessageBus::subscribe(MessageType::FreeTerroristsGift, [this]() {
            freeTerrorists();
        })
    );
}

void GameState::freeTerrorists() const {
    for (auto const& i : m_jails)
        i->freeAll();
}

sf::Vector2u GameState::getWinSize(){
    return m_stateManager.getWin().getSize();
}

GameState::~GameState() {
    for (auto& i : m_subscription) { 
        i(); 
    }
}
