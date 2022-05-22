#pragma once


#include "State.h"

#include "Game/MovingObjects.h"
#include "Game/Bomb.h"
#include "Game/Gift.h"

class GameState: public State{
public:
    using State::State;

    void init( ) override {
        m_moving.push_back(std::make_unique<Bomb>());
        // std::make_unique<Gift>();
        auto g = Gift();
        g.onEvent(sf::Event::MouseButtonReleased, [&](){
            // check if clicked
            // run animation
            
            // std::erase_if(m_moving, [](auto item){ return item == g});
        });
    }

    virtual void handleEvent(const sf::Event&) override {};
    virtual void update(const sf::Time& dt) override {
        for(auto& m: m_moving){
            m->update(dt);
        }

        for(auto& m: m_moving){
            // check if need to remove
            // check if exploded
            // check if collided
            // for(auto& n: m_nonMoving){

            // }
        }
    };
    virtual void draw(sf::RenderTarget& win) const override {
        for(auto& m: m_moving){
            m->draw(win);
        }
    };

private:
    std::vector<std::unique_ptr<MovingObjects>> m_moving;
};