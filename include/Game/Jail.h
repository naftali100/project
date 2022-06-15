#ifndef __JAIL_H__
#define __JAIL_H__

#pragma once

#include "Colors.h"
#include "Game/Bomb.h"
#include "Game/Entity.h"
#include "LevelParams.h"
#include "MessageBus.h"
#include "MovingObjects.h"

class Jail : public Entity {
public:
    Jail(const LevelParams& p, const sf::Color& color, const sf::Vector2f& pos);

    void setColor(const sf::Color& c);
    sf::Color getColor() const;

    void addBomb(Bomb* b);

    void update(const sf::Time& dt) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

    sf::FloatRect getGlobalBounds() const override;

    bool isBroken() const;
    void freeAll();

    ~Jail();

private:
    sf::Sprite m_sprite{TextureHolder::get(Textures::Jail)};
    sf::Color m_color = Colors::White;
    std::vector<Bomb*> m_bombs;
    int m_bombBuffer;
    std::vector<Func> m_subs;
    bool m_isJailBreak = false;
    // bool m_isActive = false;
    Timer m_jailBreakTimer;
    Explosion m_explosion;
};
#endif  // __JAIL_H__