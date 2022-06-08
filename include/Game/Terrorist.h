#pragma once

class Gift2 : public Animateable {
public:
    Gift2(const sf::Vector2f position = sf::Vector2f{ 200.0, 200.0 })
        :Animateable::Animateable(3, 4, Textures::Gift2, 0.8, position, 10000)
    {
        // m_sprite.scale(.25 / 2.f,.25/ 2.f);
    }
};