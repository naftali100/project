#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Resources.h"
#include "Animateable.h"

class Gift2 : public Animateable {
public:
    Gift2(const sf::Vector2f position = sf::Vector2f{ 200.0, 200.0 })
    :Animateable::Animateable(3,4,Textures::Gift2, 0.1, position, 0)
    {
       // m_sprite.scale(.25 / 2.f,.25/ 2.f);
    }
};