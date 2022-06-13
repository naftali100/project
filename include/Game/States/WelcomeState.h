#pragma once

#include "GameState.h"
#include "Gui/button.h"
#include "Resources.h"
#include "SfmlUtil.h"
#include "State.h"
#include "StateManager.h"
#include "setState.h"

class WelcomeState : public State {
public:
    using State::State;

    void init() override {
        LOGV;
        m_cam.setView(m_stateManager.getWin().getDefaultView());
        m_cam.setInitialView();

        auto btnXPos = ((float)m_stateManager.getWin().getSize().x / 2) - sf::util::getGlobalCenter(m_btn).x;
        m_setBtn.setPosition({btnXPos, 100});
        m_btn.setPosition({btnXPos, 200});
        m_setBtn.setFunction([this]() { m_stateManager.pushState(std::make_unique<setState>(m_stateManager)); });
        m_btn.setFunction([this]() { m_stateManager.pushState(std::make_unique<GameState>(m_stateManager)); });
        LOGV;
    }

    void handleEvent(const sf::Event& e) override {
        LOGV << "start";
        m_btn.handleEvent(e);
        m_setBtn.handleEvent(e);
        LOGV << "finish";
    }
    void update(const sf::Time& dt) override {
        LOGV;
        m_btn.update();
        m_setBtn.update();
        ImGui::ShowDemoWindow();
        if (ImGui::Button("close state")) {
            m_stateManager.popState();
            return;
        }
        LOGV;
    }
    void draw(sf::RenderTarget& win) const override {
        m_cam.draw(win);
        // win.draw(sf::Sprite{TextureHolder::get(Textures::Test)});
        auto newSize = (sf::Vector2f) m_cam.getView().getSize();
        // auto newSize = (sf::Vector2f) m_stateManager.getWin().getView().getSize();
        sf::RectangleShape rec{(sf::Vector2f)TextureHolder::get(Textures::Test).getSize()};
        rec.setTexture(&TextureHolder::get(Textures::Test));
        // rec.setSize(newSize);
        auto oldSize = rec.getSize();
        rec.scale(newSize.y / oldSize.y, newSize.y / oldSize.y);
        win.draw(rec);
        m_btn.draw(win);
        m_setBtn.draw(win);
    }

private:
    gui::Button m_btn{"start the game!!"};
    gui::Button m_setBtn{"settings"};
    Camera m_cam;
};