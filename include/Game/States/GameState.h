#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__
#pragma once

#include "Game/Explosion.h"
#include "Animation.h"
#include "CollisionHandler.h"
#include "Game/MovingObjects.h"
#include "State.h"
#include "StateComponents/Camera.h"
#include <functional>

class GameState : public State {
public:
    //using State::State;
    GameState(StateManager& sm)
        :State::State(sm), m_explosion({ 200, 200 }) {
        
    }

    void init() override;
    void initLayout();
    void initJail();

    void handleEvent(const sf::Event&) override;
    void update(const sf::Time& dt) override;
    void draw(sf::RenderTarget& win) const override;
    //auto func = [&]() {m_stateManager.replaceState(std::make_unique<WelcomeState>(m_stateManager)); };

private:
    void handleCollisions(const sf::Time&);
    sf::Vector3f getManifold(const sf::FloatRect& overlap, const sf::Vector2f& colNormal) const;

private:
    std::vector<std::unique_ptr<MovingObjects>> m_moving;
    std::vector<std::unique_ptr<Entity>> m_static;
    std::vector<std::unique_ptr<Explosion>> m_explosions;
    CollisionHandler m_col;
    Camera m_cam;

    sf::Sprite m_stars;
    Animation m_starAnimation{m_stars};
    Explosion m_explosion;
    MyTimer m_timer;
    int m_lives = 3;
};

#endif  // __GAMESTATE_H__