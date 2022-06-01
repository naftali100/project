#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Resources.h"
#include "Animateable.h"

class Explosion : public Animateable {
public:
    Explosion(const sf::Vector2f position = sf::Vector2f{ 0.0, 0.0 })
    :Animateable::Animateable(2,4,Textures::Explosion, 0.08, position)
    {
        m_sprite.scale(.25 / 2.f,.25/ 2.f);
    }
};