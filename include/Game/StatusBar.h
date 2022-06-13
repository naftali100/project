#ifndef __STATUSBAR_H__
#define __STATUSBAR_H__
#pragma once

#include "Resources.h"

class StatusBar {
public:
    explicit StatusBar(int& lives, int& score): m_lives(lives), m_score(score){};
    void update();
    void draw(sf::RenderTarget& win) const;

private:
    int& m_lives;
    int& m_score;
};

#endif // __STATUSBAR_H__
