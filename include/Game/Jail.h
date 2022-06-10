#ifndef __JAIL_H__
#define __JAIL_H__

#pragma once

#include "Game/Bomb.h"
#include "Game/Entity.h"
#include "Colors.h"
//#include "Animateable.h"
#include "MessageBus.h"
#include "LevelParams.h"
#include "MovingObjects.h"

// class Jail : public Animateable {
class Jail : public Entity {
public:
    explicit Jail(const LevelParams& p);
    void setColor(const sf::Color& c);
    sf::Color getColor() const;

    void addBomb(Bomb* b);

    void update(const sf::Time& dt) override {
        m_jailBreakTimer.update(dt);
    };

    sf::FloatRect getGlobalBounds() const override;

    bool isBreaked();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void freeAll();

    ~Jail();

private:
    sf::Color m_color = Colors::White;
    std::vector<Bomb*> m_bombs;
    sf::Sprite m_sprite { TextureHolder::get(Textures::Jail) };
    int m_bombBuffer;
    std::vector<MessageBus::Func> m_subs;
    bool m_isJailBreak = false;
    Timer m_jailBreakTimer;
};
#endif  // __JAIL_H__