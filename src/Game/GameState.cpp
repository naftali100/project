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
    initState();
    initLayout();
    initJail();
    initDoors();
    registerMessageHandlers();
}

void GameState::initState(){
    m_cam.setView(m_stateManager.getWin().getDefaultView());
    m_cam.setInitialView();
    m_cam.setWinRatio(m_stateManager.getWin().getSize());
    m_cam.setResizeStrategy(LatterBox);

    m_stars.setTexture(TextureHolder::get(Textures::Stars));

    static float spawnInterval = 3;
    m_spawnTimer.set(
        [this]() {
            m_spawnTimer.setTime(sf::seconds(Random::rnd(0.f, m_params.m_spawnRate)));
            spawnBomb();
        },
        m_params.m_spawnRate);

    m_starAnimation.initFramesWithFixedSize(m_stars.getTexture()->getSize(), 1, 9, 0.1f);

    for (int i = 0; i < 3; i++) {  // TODO: replace this with std "do_it_n_times" function
        // spawn bomb
        spawnBomb();
        // spawnGift();
    }
}

void GameState::initJail() {
    auto winSize = m_stateManager.getWin().getSize();
    // left jail
    auto j = std::make_unique<Jail>();
    j->setColor(Colors::STD_COLORS[0]);
    m_static.push_back(std::move(j));
    m_static.back()->setOrigin(sf::util::getGlobalCenter(*m_static.back().get()));
    m_static.back()->setPosition(100, (float)winSize.y / 2);

    // right jail
    j = std::make_unique<Jail>();
    j->setColor(Colors::STD_COLORS[1]);
    m_static.push_back(std::move(j));
    m_static.back()->setOrigin(sf::util::getGlobalCenter(*m_static.back().get()));
    m_static.back()->setPosition((float)winSize.x - 100, (float)winSize.y / 2);

    // bottom right jail
    j = std::make_unique<Jail>();
    j->setColor(Colors::STD_COLORS[2]);
    j->setOrigin(sf::util::getGlobalCenter(*j));
    j->setPosition((float)winSize.x / 2, (float)winSize.y - (j->getGlobalBounds().height /2));
    m_static.push_back(std::move(j));

    // bottom left jail
    j = std::make_unique<Jail>();
    j->setColor(Colors::STD_COLORS[3]);
    j->setOrigin(sf::util::getGlobalCenter(*j));
    j->setPosition((float)winSize.x / 2 + j->getGlobalBounds().width + 10, (float)winSize.y - (j->getGlobalBounds().height /2));
    m_static.push_back(std::move(j));
}

void GameState::initLayout() {
    auto winSize = m_stateManager.getWin().getSize();

    // left wall
    m_static.push_back(std::make_unique<Wall>());
    auto const& w_left = m_static.back();
    w_left->setPosition(-100, -100);
    w_left->setSize(100, (float)winSize.y + 200);

    // bottom wall
    m_static.push_back(std::make_unique<Wall>());
    auto const& w_bottom = m_static.back();
    w_bottom->setPosition(-100, (float)winSize.y);
    w_bottom->setSize((float)winSize.x + 200, 100);

    // right wall
    m_static.push_back(std::make_unique<Wall>());
    auto const& w_right = m_static.back();
    w_right->setPosition((float)winSize.x, -100);
    w_right->setSize(100, (float)winSize.y + 200);

    // upper wall
    m_static.push_back(std::make_unique<Wall>());
    auto const& w_upper = m_static.back();
    w_upper->setPosition(-100, -100);
    w_upper->setSize((float)winSize.x + 200, 100);
}

void GameState::initDoors() {
    auto winSize = m_stateManager.getWin().getSize();

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

    // for debugging!
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

    auto tmp = m_params;
    ImGui::SliderFloat("bomb speed", &m_params.m_speed, 100, 1000);
    ImGui::SliderFloat("spawn rate", &m_params.m_spawnRate, 1, 10);
    ImGui::SliderInt("color amount", &m_params.m_colors, 1, 3);
    ImGui::SliderInt("non jailed at same time", &m_params.m_maxBomb, 5, 10);
    if(m_params != tmp){
        MessageBus::notify<LevelParams*>(MessageType::LevelParamsUpdated, &m_params);
    }
    if (ImGui::Button("add life")) {
        m_lives++;
    }
    if (ImGui::Button("spawn bomb")) {
        for(auto i =0; i < 10; i++)
            spawnBomb();
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
        m_stateManager.popState();
        return;
    }
    if (ImGui::Button("exit program")) {
        m_stateManager.stop();
        return;
    }

    ImGui::Text("bomb count: %lu", m_moving.size());
    MessageBus::update();
    MessageBus::update<LevelParams*>();

    m_cam.update(dt);
    m_starAnimation.update(dt);
    m_spawnTimer.update(dt);
    for (auto const& i : m_doors) { i->update(dt); }
    for (auto const& i : m_moving) { i->update(dt); }
    for (auto const& i : m_static) { i->update(dt); }
    for (auto const& i : m_explosions) { i->update(dt); }

    handleCollisions(dt);

    std::erase_if(m_moving, [](const auto& item) { return item->isTimeout(); });

    if (m_nonJailedBomb > m_params.m_maxBomb) {
        m_spawnTimer.pause();
    }
    else {
        m_spawnTimer.resume();
    }

    LOGV;
};

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
    sf::FloatRect overlap;

    for (auto const& m : m_moving) {
        // check if need to remove  - for this there is erase_if. the lambda will do it
        // check if exploded - you don't have to. the lambda expression will do it
        // check if collided
        for (auto const& n : m_static) {
            if (m->getGlobalBounds().intersects(n->getGlobalBounds(), overlap)) {
                // method A.
                auto collisionNormal = n->getPosition() - m->getPosition();
                auto manifold = getManifold(overlap, collisionNormal);
                m->handleCollision(n.get(), manifold);
                // method B.
                auto f = m_col.getCollisionHandler(*m, *n);
                if (f != nullptr) {
                    (*f)(*m, *n);
                }
            }
        }
    }
}

void GameState::spawnBomb() {
    auto winSize = m_stateManager.getWin().getSize();
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
    auto winSize = m_stateManager.getWin().getSize();
    auto b = std::make_unique<Gift>();
    b->setDirection({static_cast<float>(Random::rnd(1.0, 100.0)), static_cast<float>(Random::rnd(1.0, 100.0))});
    b->setPosition((float)Random::rnd(10, winSize.x - 10), (float)Random::rnd(10, winSize.y - 10));
    m_moving.push_back(std::move(b));
}

void GameState::registerMessageHandlers() {
    MessageBus::subscribe(MessageType::BombJailed, [this]() {
        m_nonJailedBomb--;
    });
    MessageBus::subscribe(MessageType::BombTimedout, [this]() {
        m_lives--;
        m_nonJailedBomb--;
    });
    MessageBus::subscribe<Bomb*>(MessageType::BombRemoveFromVector, [this](auto bomb) {
        bomb->kill();
        m_score++;
    });
    MessageBus::subscribe<int>(MessageType::ScoreGift, [this](int i){
        m_score+=i;
    });
}
