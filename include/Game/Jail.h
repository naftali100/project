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

    //void update(const sf::Time& dt) override{
    //    // no updates to handle
    //}; 
    using Animateable::update;
    using Animateable::handleEvent;
    //void handleEvent(const sf::Event& e) override {
    //    // no event to handle
    //};
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Color m_color = Colors::White;
    std::vector<Bomb*> m_bombs;
    //sf::RectangleShape m_rec;
    //sf::Sprite m_sprite;
};
#endif  // __JAIL_H__