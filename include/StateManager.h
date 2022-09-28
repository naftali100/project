#pragma once
#ifndef state_manager_h
#define state_manager_h

#include "State.h"

// implements State design pattern - https://refactoring.guru/design-patterns/state
class StateManager {
public:
    explicit StateManager(sf::RenderWindow& win);

    void pushState(StatePtr, bool deffer = true);
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
    void doUpdateStack();

    bool m_showImGuiGameWindow = true;
    sf::RenderWindow& m_win;
    std::stack<StatePtr> m_states;
    bool m_paused = false;

    // for deferred stack operations
    bool m_shouldPop = false;
    bool m_shouldPush = false;
    StatePtr m_tmpStat;
};

#endif