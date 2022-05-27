#include "Game/Bomb.h"

#include "SfmlUtil.h"

Bomb::Bomb(bool & isGameOver): m_isGameOver(isGameOver) {
    setCollisionTag(CollisionTag::bomb);
    // setPosition(100, 100);
    setSpeed(200);
    m_sprite.setTexture(TextureHolder::get(Textures::Bomb));
    setSize(sf::Vector2f(TextureHolder::get(Textures::Bomb).getSize()));
    setOrigin(getSize() / 2.f);
    m_timer.set([&]() {m_isGameOver = m_isTimeOut =  true;}, 10); //TODO: calc delay
}
