#include "Game/Bomb.h"

#include <mutex>

#include "SfmlUtil.h"

Bomb::Bomb(bool& isGameOver) : m_isGameOver(isGameOver) {
    // for ImGui to print only once.
    // see update()
    static std::once_flag flag;
    std::call_once(flag, [this]() { m_first = true; });

    setCollisionTag(CollisionTag::bomb);
    setSpeed(300);
    m_sprite.setTexture(TextureHolder::get(Textures::Bomb));
    setSize(sf::Vector2f(TextureHolder::get(Textures::Bomb).getSize()));
    setOrigin(getSize() / 2.f);
    m_timer.set([this]() { m_isGameOver = m_isTimeOut = true; }, 10);  // TODO: calc delay
}

void Bomb::update(const sf::Time& dt) {
    static float speed = m_speed;
    if (m_first)
        ImGui::SliderFloat("bomb speed", &speed, 0, 1000);

    m_speed = speed;

    // update movement
    if(!m_isDragged)
        MovingObjects::update(dt);
};

void Bomb::handleEvent(const sf::Event& e){
    // drag
    switch(e.type){
        case sf::Event::MouseButtonPressed:
            if(getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)){
                m_isDragged = true;
            }
        break;
        case sf::Event::MouseMoved:
            setPosition(e.mouseMove.x, e.mouseMove.y);
        break;
    };    
}
