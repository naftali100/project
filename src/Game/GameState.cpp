#include "Game/States/GameState.h"

#include "SfmlUtil.h"
#include "StateManager.h"

void GameState::initJail() {
    auto winSize = m_stateManager.getWin().getSize();
    // right jail
    auto j = std::make_unique<Jail>();
    j->setColor(sf::Color::Red);
    m_moving.push_back(std::move(j));
    m_moving.back()->setOrigin(sf::util::getGlobalCenter(*m_moving.back().get()));
    m_moving.back()->setPosition(50, winSize.y / 2);

    // left jail
    m_moving.push_back(std::make_unique<Jail>());
    m_moving.back()->setOrigin(sf::util::getGlobalCenter(*m_moving.back().get()));
    m_moving.back()->setPosition(winSize.x - 50, winSize.y / 2);
}
void GameState::initLayout() {}

void GameState::update(const sf::Time& dt) {
    for (auto& m : m_moving) { m->update(dt); }

    for (auto& m : m_moving) {
        // check if need to remove
        // check if exploded
        // check if collided
        // for(auto& n: m_nonMoving){

        // }
    }

    if (ImGui::Button("exit")) {
        m_stateManager.popState();
        return;
    }
};
