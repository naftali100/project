#pragma once

class Terrorist : public Animateable{
public:
    Terrorist(const sf::Vector2f position = sf::Vector2f{ 1000.0, 400.0 })
        :Animateable::Animateable(3, 4, Textures::Terrorist, 0.08, position)
    {
        m_sprite.scale(.25 / 2.f,.25/ 2.f);
    }
};