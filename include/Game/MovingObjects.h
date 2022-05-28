#pragma once

#include <cmath>
#include "Colors.h"
#include "Game/Entity.h"
#include "SfmlUtil.h"
#include "Timer.h"

class MovingObjects : public Entity {
public:
    void setDirection(const sf::Vector2f& v) {
        m_direction = sf::util::normalize(v);
    }

    void setSpeed(float s) {
        m_speed = s;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(m_sprite, states);
    }

    void flicker() {
        //flickering obj
        m_sprite.setColor(Colors::STD_COLORS[m_flicker]);
        m_flicker++;
        m_flicker %= Colors::STD_COLORS.size();
    }

    void runAnimation() {
        // run animation
    }

    // that to do when timeout
    // virtual void timeoutAction() = 0;

    bool isTimeout() {
        return m_isTimeOut;
    };
    // bomb and gifts handle collision in the same way
    void handleCollision(Entity* e, const sf::Vector3f& manifold) override;

    virtual ~MovingObjects() = default;

    void update(const sf::Time& dt) override {
        m_timer.update(dt);
        if (m_timer.asSeconds() < 3)
            flicker();
        move(m_direction * m_speed * dt.asSeconds());
    }

protected:
    sf::Vector2f m_direction;  // < normalized vector
    bool m_isSelected;
    MyTimer m_timer;
    float m_speed;
    bool m_isTimeOut = false;
    sf::Sprite m_sprite;

private:
    void resolveCollision(const sf::Vector3f& manifold);
    int m_flicker = 0;
};