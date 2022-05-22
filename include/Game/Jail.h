#pragma once

#include "Game/Entity.h"

class Jail : public Entity {
public:

private:
    sf::Color m_color;
    std::vector<Bomb*> m_bombs;
};