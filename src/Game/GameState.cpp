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
    w_left->setPosition(-1, -1);
    w_left->setSize(1, winSize.y + 1);

    // bottom wall
    m_static.push_back(std::make_unique<Wall>());
    auto& w_bottom = m_static.back();
    w_bottom->setPosition(-1, winSize.y);
    w_bottom->setSize(winSize.x + 5, 1);

    // right wall
    m_static.push_back(std::make_unique<Wall>());
    auto& w_right = m_static.back();
    w_right->setPosition(winSize.x - 1, -1);
    w_right->setSize(1, winSize.x + 5);

    // upper wall
    m_static.push_back(std::make_unique<Wall>());
    auto& w_upper = m_static.back();
    w_upper->setPosition(-1, -1);
    w_upper->setSize(winSize.x, 1);
}

void GameState::handleEvent(const sf::Event& e){
    m_cam.handleEvent(e);
}

void GameState::update(const sf::Time& dt) {
    m_cam.update(dt);

    for (auto& m : m_moving) { m->update(dt); }
    sf::FloatRect in;
    for (auto& m : m_moving) {
        // check if need to remove
        // check if exploded
        // check if collided
        for (auto& n : m_static) {
            // LOGI << PLOG_PRINT_VAR(m->getType()) << ". " << PLOG_PRINT_VAR(n->getType());
            if (m->getGlobalBounds().intersects(n->getGlobalBounds(), in)){
                m->handleCollision(n.get());
                auto f = col.getCollisionHandler(*m, *n); 
                if(f != nullptr){
                    (*f)(*m, *n);
                };
            }
        }
    }

    if (ImGui::Button("exit")) {
        m_stateManager.popState();
        return;
    }
};
