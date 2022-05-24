#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__
#pragma once

#include "State.h"

#include "Game/MovingObjects.h"
#include "Game/Bomb.h"
#include "Game/Gift.h"
#include "Game/Jail.h"
#include "CollisionHandler.h"
#include "StateComponents/Camera.h"

class GameState: public State{
public:
    using State::State;

    void init() override;

    void initLayout();
    void initJail();

    virtual void handleEvent(const sf::Event&) override;
    virtual void update(const sf::Time& dt) override;
    virtual void draw(sf::RenderTarget& win) const override {
        m_cam.draw(win);

        for(auto& m: m_static){
            m->draw(win);
        }
        for(auto& m: m_moving){
            m->draw(win);
        }
    };

private:
    std::vector<std::unique_ptr<Entity>> m_moving;
    std::vector<std::unique_ptr<Entity>> m_static;
    CollisionHandler col;
    Camera m_cam;
};

#endif // __GAMESTATE_H__