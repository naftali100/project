#ifndef __JAIL_H__
#define __JAIL_H__

#pragma once

#include "Game/Entity.h"
#include "Game/Bomb.h"

class Jail : public Entity {
public:
    Jail();
    void setColor(const sf::Color& c);

    sf::FloatRect getGlobalBounds() const;

    virtual void update(const sf::Time& dt) override {};
    virtual void handleEvent(const sf::Event& e){};
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Color m_color;
    std::vector<Bomb*> m_bombs;
    sf::RectangleShape m_rec;
};
#endif // __JAIL_H__