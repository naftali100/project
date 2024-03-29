#pragma once

#include "State.h"
#include "Gui/button.h"
#include "Game/LevelParams.h"
#include "Resources.h"

class SettingsState : public State {
public:
    using State::State;

    void init() override;
    void handleEvent(const sf::Event&) override;
    void update(const sf::Time& dt) override;
    void draw(sf::RenderTarget& win) const override;

private:
    void initBackground();
    void imGui();
    void setting(const std::string& title, const std::string& explanation, int* p, float min, float max);
    void level(const sf::Sprite& s, LevelParams p, const Textures::ID& texture);

private:
    sf::RectangleShape m_background;
    gui::Button m_OKBtn{ "OK" };
    LevelParams m_params;
    Textures::ID m_texture  = Textures::SkyBackground;
};

