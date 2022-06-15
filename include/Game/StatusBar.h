#ifndef __STATUSBAR_H__
#define __STATUSBAR_H__
#pragma once

#include "Animation.h"
#include "Resources.h"

class StatusBar {
public:
    StatusBar(int& lives, int& score) : m_lives(lives), m_score(score) {
        m_starAnimation.initFramesWithFixedSize(m_stars.getTexture()->getSize(), 1, 9, 0.1f);
    };
    int getLive();
    void addLive();
    void hit(int amount);
    void scorePoint(int amount);
    int getScore();

    void update(const sf::Time&);
    void draw(sf::RenderTarget& win) const;

private:
    sf::Sprite m_stars{TextureHolder::get(Textures::Stars)};
    Animation m_starAnimation{m_stars};

    int& m_lives;
    int& m_score;
};

#endif  // __STATUSBAR_H__
