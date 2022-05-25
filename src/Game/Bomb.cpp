#include "Game/Bomb.h"

Bomb::Bomb(bool & isGameOver): m_isGameOver(isGameOver) {
    setCollisionTag(CollisionTag::bomb);
    // setPosition(100, 100);
    setSpeed(200);
    m_sprite.setTexture(TextureHolder::get(Textures::Bomb));
    setSize(sf::Vector2f(TextureHolder::get(Textures::Bomb).getSize()));
    setOrigin(getSize() / 2.f);
    m_timer.setTimeout([&]() {m_isGameOver = m_isTimeOut =  true;}, 10); //TODO: calc delay
}

void Bomb::handleCollision(Entity* e) {
    switch (e->getCollisionTag()) {
        case CollisionTag::wall:
        case CollisionTag::jail:
            ImGui::Text("colliding with: %s", e->getType().c_str());
    }
}