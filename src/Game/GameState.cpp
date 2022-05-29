#include "Game/States/GameState.h"
#include "Game/Bomb.h"
#include "Game/Door.h"
#include "Game/Gift.h"
#include "Game/Jail.h"
#include "Game/Wall.h"
#include "Random.h"
#include "Resources.h"
#include "SfmlUtil.h"
#include "StateManager.h"
#include <memory>
#include "Game/States/WelcomeState.h"

void GameState::init() {
    m_cam.setView(m_stateManager.getWin().getDefaultView());
    m_cam.setInitialView();
    m_cam.setWinRatio(m_stateManager.getWin().getSize());
    m_cam.setResizeStrategy(LatterBox);

    m_stars.setTexture(TextureHolder::get(Textures::Stars));

    initJail();
    initLayout();

    // example how to use collision manager
    // col.addCollision(Bomb(m_isGameOver), Jail(), [&](const Entity& bomb, const Entity& jail) {
    //     ImGui::Text("bomb and jail collision function handler");
    // });

    sf::Vector2u textureSize = m_stars.getTexture()->getSize();

    int textureRows = 1;
    int textureCols = 9;

    m_starAnimation.initFramesWithFixedSize(textureSize, textureRows, textureCols, 0.1);

    // std::make_unique<Gift>();
    //Gift g;
    // g.onEvent(sf::Event::MouseButtonReleased, [&]() {
    //  check if clicked
    //  run animation

    // std::erase_if(m_moving, [](auto item){ return item == g});
    //});
}

void GameState::initJail() {
    auto winSize = m_stateManager.getWin().getSize();
    // left jail
    auto j = std::make_unique<Jail>();
    j->setColor(sf::Color::Red);
    m_static.push_back(std::move(j));   //it's dangerous!
    m_static.back()->setOrigin(sf::util::getGlobalCenter(*m_static.back().get()));
    m_static.back()->setPosition(50, (float)winSize.y / 2);

    // right jail
    m_static.push_back(std::make_unique<Jail>());
    m_static.back()->setOrigin(sf::util::getGlobalCenter(*m_static.back().get()));
    m_static.back()->setPosition((float)winSize.x - 50, (float)winSize.y / 2);

    for (int i = 0; i < 3; i++) {  // TODO: replace this with std "do_it_n_times" function
        // spawn bomb
        auto b = std::make_unique<Bomb>(m_isGameOver);
        b->setDirection({static_cast<float>(Random::rnd(1.0, 100.0)), static_cast<float>(Random::rnd(1.0, 100.0))});
        b->setPosition(static_cast<float>(Random::rnd(10, winSize.x - 10)),
                       static_cast<float>(Random::rnd(10, winSize.y - 10)));
        m_moving.push_back(std::move(b));

    //     // spawn gift (currently is stars image) 
    //     auto g = std::make_unique<Gift>();
    //     g->setDirection({ static_cast<float>(Random::rnd(1.0, 100.0)), static_cast<float>(Random::rnd(1.0, 100.0)) });
    //     g->setPosition(static_cast<float>(Random::rnd(10, winSize.x - 10)),
    //         static_cast<float>(Random::rnd(10, winSize.y - 10)));
    //     m_moving.push_back(std::move(g));       // dangerous
    }
}

void GameState::initLayout() {
    auto winSize = m_stateManager.getWin().getSize();

    // left wall
    m_static.push_back(std::make_unique<Wall>());
    auto const& w_left = m_static.back();
    w_left->setPosition(-10, -10);
    w_left->setSize(10, (float)winSize.y + 20);

    // bottom wall
    m_static.push_back(std::make_unique<Wall>());
    auto const& w_bottom = m_static.back();
    w_bottom->setPosition(-10, (float)winSize.y);
    w_bottom->setSize((float)winSize.x + 20, 10);

    // right wall
    m_static.push_back(std::make_unique<Wall>());
    auto const& w_right = m_static.back();
    w_right->setPosition((float)winSize.x + 10, -10);
    w_right->setSize(10, (float)winSize.y + 20);

    // upper wall
    m_static.push_back(std::make_unique<Wall>());
    auto const& w_upper = m_static.back();
    w_upper->setPosition(-10, -10);
    w_upper->setSize((float)winSize.x + 20, 10);
}

void GameState::handleEvent(const sf::Event& e) {
    m_cam.handleEvent(e);
    if (e.MouseButtonReleased)
        for (auto& item : m_moving)
            item->handleEvent(e);
}

void GameState::update(const sf::Time& dt) {
    if(m_lives <= 0){
        m_stateManager.replaceState(std::make_unique<WelcomeState>(m_stateManager));
        return;
    }
    auto winSize = m_stateManager.getWin().getSize();

    if (ImGui::Button("spawn bomb")) {
        auto b = std::make_unique<Bomb>(m_isGameOver);
        b->setDirection({static_cast<float>(Random::rnd(1.0, 100.0)), static_cast<float>(Random::rnd(1.0, 100.0))});
        b->setPosition((float)Random::rnd(10, winSize.x - 10), (float)Random::rnd(10, winSize.y - 10));
        m_moving.push_back(std::move(b));
    }
    if (ImGui::Button("spawn gift")) {
        auto g = std::make_unique<Gift>();
        g->setDirection({ static_cast<float>(Random::rnd(1.0, 100.0)), static_cast<float>(Random::rnd(1.0, 100.0)) });
        g->setPosition((float)Random::rnd(10, winSize.x - 10), (float)Random::rnd(10, winSize.y - 10));
        m_moving.push_back(std::move(g));
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

    m_cam.update(dt);
    m_starAnimation.update(dt.asSeconds());
    m_explosion.update(dt);

    handleCollisions(dt);

    auto removedCount = std::erase_if(m_moving, [](const auto& item) { return item->isTimeout(); });
    if(removedCount > 0){
        m_lives -= removedCount;
    }
};

void GameState::draw(sf::RenderTarget& win) const {
    m_cam.draw(win);  // set view

    auto localStars = m_stars;
    for(int i = 0; i < m_lives; i++){
        localStars.setPosition(100 * i, 0);
        win.draw(localStars);
    }
    
    m_explosion.draw(win, sf::RenderStates::Default);
    for (auto& m : m_static) { m->draw(win); }
    for (auto& m : m_moving) { m->draw(win, sf::RenderStates::Default); }   
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

void GameState::handleCollisions(const sf::Time& dt) {
    sf::FloatRect overlap;

    for (auto const& m : m_moving) {
        m->update(dt);
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
