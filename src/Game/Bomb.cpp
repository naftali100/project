#include "Game/Bomb.h"

#include <mutex>

#include "Game/Jail.h"
#include "Random.h"
#include "SfmlUtil.h"

Bomb::Bomb(std::vector<std::unique_ptr<Explosion>>& explosions, const LevelParams& p, const sf::Vector2f& pos,
           const sf::Vector2f& dir, sf::Vector2u winSize) :
    MovingObjects::MovingObjects(dir), m_explosions(explosions), m_winSize(winSize) {
    setPosition(pos);
    initSprite();
    
    MovingObjects::setSpeed(p.m_speed);
    m_color = Colors::STD_COLORS[Random::rnd(0, p.m_colors)];

    setCollisionTag(CollisionTag::bomb);

    m_timer.set(
        [this]() {
            MessageBus::notify(MessageType::BombTimedout);
            kill();
            m_explosions.push_back(std::make_unique<Explosion>(getPosition()));
        },
        p.m_bombTime);

    m_footStep.setGravity(-m_direction.x, -m_direction.y);
    m_footStep.setDissolutionRate(5);
	m_footStep.setParticleSpeed(50.0f);
}

void Bomb::initSprite() {
    setSpriteByDirection();
    m_animation.initFramesWithFixedSize(m_sprite.getTexture()->getSize(), 3, 4, 0.08f);
    m_animation.setFrame(0);

    float scale = 2;
    m_sprite.scale(sf::Vector2f(1, 1) / scale);
    MovingObjects::setSize(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height);
    setOrigin(MovingObjects::getSize() / 2.f);
}

void Bomb::setSpriteByDirection(){
    if(m_direction.x < 0){
        m_sprite.setTexture(TextureHolder::get(Textures::TerroristRunLeft));
    }else{
        m_sprite.setTexture(TextureHolder::get(Textures::TerroristRunRight));
    }
}

void Bomb::update(const sf::Time& dt) {
    if (m_isJailed)
        return;

    // need to update timer even if dragged
    m_timer.update(dt);
    m_animation.update(dt);

    // update movement
    if (!m_isDragged)
        MovingObjects::update(dt);

    m_footStep.setPosition(sf::util::getGlobalBottomLeft(*this));
	deffer--;
	if(deffer < 0){
        m_footStep.addParticles<TrailParticle>(1);
		deffer = 3;
	}
    m_footStep.update(dt);

    if (!sf::FloatRect(-10.0, -10.0, m_winSize.x, m_winSize.y).contains(getPosition())){
        kill();
        // this cause the game state to update the amount of non jailed bomb which is wrong if just calling kill because game state not updated about it
        MessageBus::notify(MessageType::BombJailed); 
        LOGE << "bomb out";
    }
};

void Bomb::handleEvent(const sf::Event& e) {
    // drag
    switch (e.type) {
        case sf::Event::MouseButtonPressed:
            if (getGlobalBounds().contains((float)e.mouseButton.x, (float)e.mouseButton.y)) {
                m_isDragged = true;
            }
            break;
        case sf::Event::MouseButtonReleased:
            m_isDragged = false;
            break;
        case sf::Event::MouseMoved:
            if (m_isDragged && !m_isJailed)
                setPosition((float)e.mouseMove.x, (float)e.mouseMove.y);
            break;
        default:;
    }
}

void Bomb::handleCollision(Entity* e, const sf::Vector3f& manifold) {
    if (m_isJailed) return;

    if (e->getCollisionTag() == CollisionTag::jail) {
        sf::FloatRect tempRect;
        // if intersects, and the whole entity is inside the jail
        if (getGlobalBounds().intersects(e->getGlobalBounds(), tempRect) && tempRect.width == getGlobalBounds().width &&
            tempRect.height == getGlobalBounds().height) {
            resolveCollision(e);
        }
    }

    if (!m_isDragged){
        auto tmp = m_direction;
        MovingObjects::handleCollision(e, manifold);
        if(m_direction.x != tmp.x){
            setSpriteByDirection();
        }
    }
}

void Bomb::resolveCollision(Entity* e){
    auto jail = dynamic_cast<Jail*>(e);  // needed for getting jail's color.
    if (m_color != jail->getColor()) {
        m_timer.reset();  // calls kill and add explosion
    }
    else {
        MessageBus::notify(MessageType::BombJailed);
        jail->addBomb(this);
        arrest();
        playSound();
    }
}

void Bomb::draw(sf::RenderTarget& win, sf::RenderStates states) const {
    MovingObjects::draw(win, states);
    sf::CircleShape rec;
    rec.setRadius(20);
    rec.setFillColor(m_color);
    win.draw(rec, getTransform());
    m_footStep.draw(win);
}

void Bomb::playSound(){
    static sf::Sound m_sound{SoundBufferHolder::get(SoundEffect::Jailed)};
    m_sound.play();
}