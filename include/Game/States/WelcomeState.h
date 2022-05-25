#pragma once

#include "GameState.h"
#include "Gui/button.h"
#include "Resources.h"
#include "SfmlUtil.h"
#include "State.h"

class WelcomeState : public State {
public:
    using State::State;

    void init() override {
        auto btnXPos = (m_stateManager.getWin().getSize().x / 2) - sf::util::getGlobalCenter(m_btn).x;
        m_btn.setPosition({btnXPos, 100});
        m_btn.setFunction([&]() { m_stateManager.pushState(std::make_unique<GameState>(m_stateManager), true); });

        auto s = sf::Sprite(TextureHolder::get(Textures::Player));
    }

    virtual void handleEvent(const sf::Event& e) override {
        m_btn.handleEvent(e);
    }
    virtual void update(const sf::Time& dt) override {
        m_btn.update();
        ImGui::ShowDemoWindow();
        if (ImGui::Button("close state")) {
            m_stateManager.popState();
            return;
        }
    }
    virtual void draw(sf::RenderTarget& win) const override {
        m_btn.draw(win);
    }

private:
    gui::Button m_btn{"start the game!!"};
};