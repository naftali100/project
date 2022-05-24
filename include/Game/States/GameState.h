#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

#pragma once

#include "State.h"

#include "Game/MovingObjects.h"
#include "Game/Bomb.h"
#include "Game/Gift.h"
#include "Game/Jail.h"

class GameState: public State{
public:
    using State::State;

    void init() override {
        m_moving.push_back(std::make_unique<Bomb>());

        initJail();        

        // std::make_unique<Gift>();
        auto g = Gift();
        g.onEvent(sf::Event::MouseButtonReleased, [&](){
            // check if clicked
            // run animation
            
            // std::erase_if(m_moving, [](auto item){ return item == g});
        });
    }

    void initLayout();
    void initJail();


    virtual void handleEvent(const sf::Event&) override {};
    virtual void update(const sf::Time& dt) override ;
    virtual void draw(sf::RenderTarget& win) const override {
        for(auto& m: m_moving){
            m->draw(win);
        }
    };

private:
    std::vector<std::unique_ptr<Entity>> m_moving;
};


#endif // __GAMESTATE_H__