#pragma once

#include "GameState.h"
#include "Gui/button.h"
#include "Resources.h"
#include "SfmlUtil.h"
#include "State.h"
#include "StateManager.h"
#include "setState.h"
#include "ParticleSystem.h"
#include "Game/Timer.h"

class WelcomeState : public State {
public:
    using State::State;

    void init() override;

    void handleEvent(const sf::Event& e) override;
    void update(const sf::Time& dt) override;
    void draw(sf::RenderTarget& win) const override;

private:
    gui::Button m_btn{"start the game!!"};
    gui::Button m_setBtn{"settings"};
    Camera m_cam;
    mutable ParticleSystem m_ps{m_stateManager.getWin().getSize()};
    Timer m_gravityChangeTimer;
};