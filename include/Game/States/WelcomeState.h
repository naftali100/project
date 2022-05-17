#pragma once

#include "Gui/button.h"
#include "State.h"
#include "SfmlUtil.h"
#include "imgui_internal.h"

class WelcomeState : public State {
public:
    using State::State;

    void init() override {
        auto btnXPos = (m_stateManager.getWin().getSize().x / 2) - sf::util::getGlobalCenter(m_btn).x;
        m_btn.setPosition({btnXPos, 100});
        m_btn.setFunction([&](){
            m_stateManager.pushState(std::make_unique<WelcomeState>(m_stateManager));
        });
    }

    virtual void handleEvent(const sf::Event& e) override {
        m_btn.handleEvent(e);
    }
    virtual void update(const sf::Time& dt) override {
        m_btn.update();
    }
    virtual void draw(sf::RenderTarget& win) const override {
        m_btn.draw(win);
    }

private:
    gui::Button m_btn{"start the game!!"};
};