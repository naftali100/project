#pragma once
#ifndef state_manager_h
#define state_manager_h

#include "State.h"

// implements State design pattern - https://refactoring.guru/design-patterns/state
class StateManager {
public:
    explicit StateManager(sf::RenderWindow& win);

    // TODO
    // template <typename T, typename... Args>
    // inline void pushState(Args&&... args) {
    //     pushState(std::make_unique<T>(std::forward<Args>(args)...));
    // }
    void pushState(StatePtr);
    void replaceState(StatePtr);
    void popState();

    void handleEvent(const sf::Event&);
    void update(const sf::Time& td);
    void draw(sf::RenderTarget&);

    void stop();
    bool isRunning() const;

    void setupImGuiWindow();

    // template <typename S>
    // void registerState(States::ID state);

    // StatePtr createState(States::ID);

    sf::RenderWindow& getWin();

    void pause();
    void resume();

private:
    bool m_showImGuiGameWindow = true;
    sf::RenderWindow& m_win;
    std::stack<StatePtr> m_states;
    bool m_paused = false;

    bool m_shouldPop = false; // for deferred pop
    // std::map<States::ID, std::function<StatePtr()>> m_factories;
};

// template <typename S>
// void StateManager::registerState(States::ID state) {
//     m_factories[state] = [this]() { return std::make_unique<S>(*this); };
// }

#endif