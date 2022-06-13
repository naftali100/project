#pragma once


#include "Game/LevelParams.h"
#include "State.h"
#include "StateManager.h"
#include "Gui/button.h"
#include "Resources.h"
#include "SfmlUtil.h"
#include "StateComponents/Camera.h"
#include "SfmlUtil.h"


class setState : public State {
public:
    using State::State;

    void init() override;
    void initBackground();
    void handleEvent(const sf::Event&) override;
    void update(const sf::Time& dt) override;
    void draw(sf::RenderTarget& win) const override;
private:
    sf::RectangleShape m_background;
    gui::Button m_OKBtn{ "OK" };
    Camera m_cam;


};
