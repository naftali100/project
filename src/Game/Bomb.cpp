#include "Game/Bomb.h"

Bomb::Bomb() {
    setCollisionTag(CollisionTag::bomb);
    // setPosition(100, 100);
    setSpeed(200);
    m_sprite.setTexture(TextureHolder::get(Textures::Bomb));
    setSize(sf::Vector2f(TextureHolder::get(Textures::Bomb).getSize()));
    setOrigin(getSize() / 2.f);
}

void Bomb::handleCollision(Entity* e) {
    switch (e->getCollisionTag()) {
        case CollisionTag::wall:
        case CollisionTag::jail:
            ImGui::Text("colliding with: %s", e->getType().c_str());
    }
}