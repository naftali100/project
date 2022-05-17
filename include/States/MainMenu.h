#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H

#include "State.h"
#include "Log.h"

class MainMenu : public State {
public:
    using State::State;

    virtual void handleEvent(const sf::Event&) override{};
    virtual void update(const sf::Time&) override {
        if (ImGui::Button("show demo window")) {
            m_showDemo = true;
        }

        if (m_showDemo) {
            ImGui::ShowDemoWindow(&m_showDemo);
        }

        if (ImGui::Button("exit current state")) {
            m_stateManager.popState();
            return;
        }

        if (ImGui::Button("exit")) {
            m_stateManager.stop();
            return;
        }

        ImGui::Checkbox("pause current state when pushing new state?", &m_pauseOnPush);
        ImGui::Checkbox("show game window", &m_stateManager.m_showImGuiGameWindow);

        LOGD;
    };

    virtual void draw(sf::RenderTarget&) const override{};

private:
    // when pushing new state, pause the current or replace it with new one?
    bool m_pauseOnPush = true;
    bool m_showDemo = false;
};

#endif