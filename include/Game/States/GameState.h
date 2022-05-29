#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__
#pragma once


#include "Animation.h"
#include "CollisionHandler.h"
#include "Game/MovingObjects.h"
#include "State.h"
#include "StateComponents/Camera.h"

class GameState : public State {
public:
    using State::State;

    void init() override;
    void initLayout();
    void initJail();

    void handleEvent(const sf::Event&) override;
    void update(const sf::Time& dt) override;
    void draw(sf::RenderTarget& win) const override;

private:
    void handleCollisions(const sf::Time&);
    sf::Vector3f getManifold(const sf::FloatRect& overlap, const sf::Vector2f& colNormal) const;

private:
    std::vector<std::unique_ptr<MovingObjects>> m_moving;
    std::vector<std::unique_ptr<Entity>> m_static;
    CollisionHandler m_col;
    Camera m_cam;
    bool m_isGameOver = false;

    sf::Sprite m_stars;
    Animation m_starAnimation{m_stars};
};

#endif  // __GAMESTATE_H__