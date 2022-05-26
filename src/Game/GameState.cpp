#include "Game/States/GameState.h"

#include "Game/Wall.h"
#include "SfmlUtil.h"
#include "StateManager.h"
#include "Random.h"

void GameState::init() {
    m_cam.setView(m_stateManager.getWin().getDefaultView());
    m_cam.setInitialView();
    m_cam.setWinRatio(m_stateManager.getWin().getSize());
    m_cam.setResizeStrategy(LatterBox);

    initJail();
    initLayout();

    // example how to use collision manager
    // col.addCollision(Bomb(m_isGameOver), Jail(), [&](const Entity& bomb, const Entity& jail) {
    //     ImGui::Text("bomb and jail collision function handler");
    // });

    sf::Vector2u textureSize = m_stars.getTexture()->getSize();

    int textureRows = 1;
    int textureCols = 9;

    int FrameXSize = textureSize.x / textureCols;
    int FrameYSize = textureSize.y / textureRows;

    for (int i = 0; i < textureRows; i++) {
        for (int j = 0; j < textureCols; j++) {
            m_starAnimation.addFrame({sf::IntRect(j * FrameXSize, i * FrameYSize, FrameXSize, FrameYSize), 0.1});
        }
    }

    // std::make_unique<Gift>();
    Gift g;
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
    m_static.push_back(std::move(j));
    m_static.back()->setOrigin(sf::util::getGlobalCenter(*m_static.back().get()));
    m_static.back()->setPosition(50, winSize.y / 2);

    // right jail
    m_static.push_back(std::make_unique<Jail>());
    m_static.back()->setOrigin(sf::util::getGlobalCenter(*m_static.back().get()));
    m_static.back()->setPosition(winSize.x - 50, winSize.y / 2);

    // spawn bomb
    for(int i = 0; i < 150; i++){  // TODO: replace this with std "do_it_n_times" function
        auto b = std::make_unique<Bomb>(m_isGameOver);
        b->setDirection({Random::rnd(1.f,100.f), Random::rnd(1.f,100.f)});
        b->setPosition(Random::rnd(10, winSize.x - 10), Random::rnd(10, winSize.y - 10));
        m_moving.push_back(std::move(b));
    }
}

void GameState::initLayout() {
    auto winSize = m_stateManager.getWin().getSize();
    // left wall
    m_static.push_back(std::make_unique<Wall>());
    auto& w_left = m_static.back();
    w_left->setPosition(-10, -10);
    w_left->setSize(10, winSize.y + 20);

    // bottom wall
    m_static.push_back(std::make_unique<Wall>());
    auto& w_bottom = m_static.back();
    w_bottom->setPosition(-10, winSize.y);
    w_bottom->setSize(winSize.x + 20, 10);

    // right wall
    m_static.push_back(std::make_unique<Wall>());
    auto& w_right = m_static.back();
    w_right->setPosition(winSize.x + 10, -10);
    w_right->setSize(10, winSize.y + 20);

    // upper wall
    m_static.push_back(std::make_unique<Wall>());
    auto& w_upper = m_static.back();
    w_upper->setPosition(-10, -10);
    w_upper->setSize(winSize.x + 20, 10);
}

void GameState::handleEvent(const sf::Event& e) {
    m_cam.handleEvent(e);
}

void GameState::update(const sf::Time& dt) {
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

    sf::FloatRect overlap;
    for (auto& m : m_moving) {
        m->update(dt);
        // check if need to remove
        // check if exploded
        // check if collided
        for (auto& n : m_static) {
            // LOGI << PLOG_PRINT_VAR(m->getType()) << ". " << PLOG_PRINT_VAR(n->getType());
            if (m->getGlobalBounds().intersects(n->getGlobalBounds(), overlap)) {
                // A.
                auto collisionNormal = n->getPosition() - m->getPosition();
                auto manifold = getManifold(overlap, collisionNormal);
                m->handleCollision(n.get(), manifold);
                // B.
                auto f = col.getCollisionHandler(*m, *n);
                if (f != nullptr) {
                    (*f)(*m, *n);
                };
            }
        }
    }
};

void GameState::draw(sf::RenderTarget& win) const {
    m_cam.draw(win);  // set view

    m_stars.setPosition(0, 0);
    win.draw(m_stars);
    m_stars.setPosition(100, 0);
    win.draw(m_stars);
    m_stars.setPosition(200, 0);
    win.draw(m_stars);

    for (auto& m : m_static) { m->draw(win); }
    for (auto& m : m_moving) { m->draw(win); }
};

// from: https://gist.github.com/fallahn/f81d23137409313e7de6
sf::Vector3f GameState::getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal) {
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
