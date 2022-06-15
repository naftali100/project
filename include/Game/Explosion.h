#pragma once
#include "Animateable.h"
#include "Animation.h"
#include "Entity.h"
#include "Resources.h"

// TODO: change inheritance
class Explosion : public Animateable {
public:
    Explosion(const sf::Vector2f position = sf::Vector2f{0.0, 0.0}) :
        Animateable::Animateable(2, 4, Textures::Explosion, 0.08, position, 0) {
        m_sprite.scale(.25 / 2.f, .25 / 2.f);
    }
};