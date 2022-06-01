#ifndef __JAIL_H__
#define __JAIL_H__

#pragma once

#include "Game/Bomb.h"
#include "Game/Entity.h"
#include "Colors.h"
#include "Animateable.h"

class Jail : public Animateable {
public:
    Jail();
    void setColor(const sf::Color& c);
    sf::Color getColor() const;

    sf::FloatRect getGlobalBounds() const override;

    void addBomb(Bomb* b){
        m_bombs.push_back(b);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Color m_color = Colors::White;
    std::vector<Bomb*> m_bombs;
};
#endif  // __JAIL_H__