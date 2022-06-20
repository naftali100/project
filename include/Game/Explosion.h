#pragma once
#include "Animateable.h"
#include "Animation.h"
#include "Entity.h"
#include "Resources.h"

//constants:
const float DEFAULT_TIME = 0.8;
const int EXP_ROWS = 2;
const int EXP_COLS = 4;
const sf::Vector2f DEFAULT_SCALE = { .25 / 2.f, .25 / 2.f };

// TODO: change inheritance
class Explosion : public Animateable {
public:
    Explosion(const sf::Vector2f position = sf::Vector2f{0.0, 0.0}) :
        Animateable::Animateable(EXP_ROWS, EXP_COLS,
            Textures::Explosion, DEFAULT_TIME, position, 0) {
        m_sprite.scale(DEFAULT_SCALE);
    }
};