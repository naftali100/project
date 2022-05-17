#pragma once
#ifndef state_h
#define state_h

#include <SFML/Graphics.hpp>
#include <memory>

class StateManager;

// template for states
class State {
public:
    State(StateManager& sm) : m_stateManager(sm){};
    /* for state init when `using State::State`. called by stateManager */
    virtual void init(){}; 
    virtual void handleEvent(const sf::Event&) = 0;
    virtual void update(const sf::Time&) = 0;
    virtual void draw(sf::RenderTarget&) const = 0;

    virtual ~State() = default;

protected:
    StateManager& m_stateManager;
};

typedef std::unique_ptr<State> StatePtr;

#endif