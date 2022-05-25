#include "Game/States/GameState.h"

#include "Game/Wall.h"
#include "SfmlUtil.h"
#include "StateManager.h"

void GameState::init() {
    m_cam.setView(m_stateManager.getWin().getDefaultView());
    m_cam.setInitialView();
    m_cam.setWinRatio(m_stateManager.getWin().getSize());
    m_cam.setResizeStrategy(LatterBox);

    initJail();
    initLayout();

    col.addCollision(Bomb(), Jail(), [&](const Entity& bomb, const Entity& jail) {
        ImGui::Text("bomb and jail collision function handler");
    });

    // std::make_unique<Gift>();
    auto g = Gift();
    g.onEvent(sf::Event::MouseButtonReleased, [&]() {
        // check if clicked
        // run animation

        // std::erase_if(m_moving, [](auto item){ return item == g});
    });
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

    auto b = std::make_unique<Bomb>();
    b->setDirection({50, winSize.y / 2.f});
    m_moving.push_back(std::move(b));
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

    sf::FloatRect in;
    for (auto& m : m_moving) {
        m->update(dt);
        // check if need to remove
        // check if exploded
        // check if collided
        for (auto& n : m_static) {
            // LOGI << PLOG_PRINT_VAR(m->getType()) << ". " << PLOG_PRINT_VAR(n->getType());
            if (m->getGlobalBounds().intersects(n->getGlobalBounds(), in)) {
                // A.
                m->handleCollision(n.get());
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

    for (auto& m : m_static) { m->draw(win); }
    for (auto& m : m_moving) { m->draw(win); }
};