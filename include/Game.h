#pragma once
#ifndef GAME_H
#define GAME_H

#include "StateManager.h"

class Game {
public:
    Game();
    void run();
    void loadResources();
    void initImGui();

    void processEvents();
    void update(sf::Time elapsedTime);
    void showStatWin();
    void draw();

private:
    sf::RenderWindow m_win;
    sf::Font m_font;
    std::map<sf::Keyboard::Key, bool> pressedKeys;
    StateManager m_stateManager;
};

#endif