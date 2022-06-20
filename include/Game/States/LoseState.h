#ifndef __LOSESTATE_H__
#define __LOSESTATE_H__
#pragma once

#include "State.h"
#include "Gui/button.h"
#include "WelcomeState.h"

class LoseState : public State{
public:
    using State::State;

    void init() override {
        m_btn.setPosition({m_stateManager.getWin().getSize().x / 2 - m_btn.getGlobalBounds().width, 500});
        m_btn.setFunction([this](){
            m_stateManager.replaceState(std::make_unique<WelcomeState>(m_stateManager));
        });
        m_loseSound.setVolume(70);
        m_loseSound.play();
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
    gui::Button m_btn;
    sf::Sound m_loseSound{SoundBufferHolder::get(SoundEffect::Lose)};
};

#endif // __LOSESTATE_H__