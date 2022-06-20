#ifndef __LOSESTATE_H__
#define __LOSESTATE_H__
#pragma once

#include "Gui/button.h"
#include "State.h"
#include "WelcomeState.h"

class LoseState : public State {
public:
    using State::State;

    void init() override {
        m_btn.setPosition({(m_stateManager.getWin().getSize().x / 2) - m_btn.getGlobalBounds().width, m_stateManager.getWin().getSize().y - 200.f});
        m_btn.setFunction([this]() { m_stateManager.replaceState(std::make_unique<WelcomeState>(m_stateManager)); });
        m_loseSound.setVolume(70);
        m_loseSound.play();
        initBackground();
    }

    virtual void handleEvent(const sf::Event& e) override {
        m_btn.handleEvent(e);
    }
    virtual void update(const sf::Time& dt) override {
        m_btn.update();
    }
    virtual void draw(sf::RenderTarget& win) const override {
        win.draw(m_background);
        if(ImGui::BeginOnlyWidget()){
            if(ImGui::Button("main menu")){
                m_stateManager.replaceState(std::make_unique<WelcomeState>(m_stateManager)); 
                ImGui::End();
                return;
            }
            ImGui::SetWindowPos({
                (m_stateManager.getWin().getSize().x / 2) - (ImGui::GetItemRectSize().x / 2),
                m_stateManager.getWin().getSize().y - 200.f
            });
        }
        ImGui::End();
    }

    void initBackground() {
        m_background.setSize((sf::Vector2f)TextureHolder::get(Textures::GameOver).getSize());
        m_background.setTexture(&TextureHolder::get(Textures::GameOver));
        // scale to window size
        auto oldSize = m_background.getSize();
        auto newSize = (sf::Vector2f)m_stateManager.getWin().getView().getSize();
        m_background.scale(newSize.x / oldSize.x, newSize.x / oldSize.x);
    }

private:
    gui::Button m_btn{ "main menu" };
    sf::Sound m_loseSound{SoundBufferHolder::get(SoundEffect::Lose)};
    sf::RectangleShape m_background;
};

#endif  // __LOSESTATE_H__