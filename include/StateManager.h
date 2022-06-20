#pragma once
#ifndef state_manager_h
#define state_manager_h

#include "State.h"

// implements State design pattern - https://refactoring.guru/design-patterns/state
class StateManager {
public:
    explicit StateManager(sf::RenderWindow& win);

    void pushState(StatePtr);
    void replaceState(StatePtr);
    void popState();

    void handleEvent(const sf::Event&);
    void update(const sf::Time& td);
    void draw(sf::RenderTarget&);

    void stop();
    bool isRunning() const;

    void setupImGuiWindow();

    sf::RenderWindow& getWin();

    void pause();
    void resume();

private:
    bool m_showImGuiGameWindow = true;
    sf::RenderWindow& m_win;
    std::stack<StatePtr> m_states;
    bool m_paused = false;

    bool m_shouldPop = false;  // for deferred pop
};

#endif