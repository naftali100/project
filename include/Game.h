#pragma once
#ifndef GAME_H
#define GAME_H

#include "StateManager.h"

class Game {
public:
    Game();
    void run();
    void loadResources() const;
    void initImGui();

    void processEvents();
    void update(sf::Time elapsedTime);
    void showStatWin() const;
    void draw();

private:
    sf::RenderWindow m_win;
    StateManager m_stateManager;
};

#endif