#pragma once
#ifndef DEMO_STATE_H
#define DEMO_STATE_H

#include "State.h"

class DemoState: public State{
public:
    using State::State;
    virtual void handleEvent(const sf::Event&) override{};
    virtual void update(const sf::Time&) override{};
    virtual void draw(sf::RenderTarget&) const override{};
};
#endif