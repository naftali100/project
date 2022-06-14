#ifndef __JAIL_H__
#define __JAIL_H__

#pragma once

#include "Game/Bomb.h"
#include "Game/Entity.h"
#include "Colors.h"
#include "MessageBus.h"
#include "LevelParams.h"
#include "MovingObjects.h"

class Jail : public Entity {
public:
    explicit Jail(const LevelParams& p);
    
    void setColor(const sf::Color& c);
    sf::Color getColor() const;

    void addBomb(Bomb* b);

    void update(const sf::Time& dt) override;
    void draw(sf::RenderTarget& target,
        sf::RenderStates states = sf::RenderStates::Default) const override;

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
    //bool m_isActive = false;
    Timer m_jailBreakTimer;
    Explosion m_explosion;
};
#endif  // __JAIL_H__