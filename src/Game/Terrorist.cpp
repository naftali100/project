#include "Game/Terrorist.h"

#include "Game/Jail.h"
#include "Random.h"
#include "SfmlUtil.h"

Terrorist::Terrorist(std::vector<std::unique_ptr<Explosion>>& explosions, const LevelParams& p, const sf::Vector2f& pos,
           const sf::Vector2f& dir, sf::Vector2u winSize) :
    MovingObjects::MovingObjects(dir), m_explosions(explosions), m_winSize(winSize) {
    setPosition(pos);
    initSprite();
    
    MovingObjects::setSpeed(p.m_speed);
    m_color = Colors::STD_COLORS[Random::rnd(0, p.m_colors)];

    setCollisionTag(CollisionTag::bomb);

    m_timer.set(
        [this]() {
            MessageBus::notify(MessageType::TerroristTimedout);
            kill();
            m_explosions.push_back(std::make_unique<Explosion>(getPosition()));
        },
        p.m_bombTime);

    m_footStep.setGravity(-m_direction.x, -m_direction.y);
    m_footStep.setDissolutionRate(5);
	m_footStep.setParticleSpeed(50.0f);
}

void Terrorist::initSprite() {
    setSpriteByDirection();
    m_animation.initFramesWithFixedSize(m_sprite.getTexture()->getSize(), 3, 4, 0.08f);
    m_animation.setFrame(0);

    float scale = 2;
    m_sprite.scale(sf::Vector2f(1, 1) / scale);
    MovingObjects::setSize(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height);
    setOrigin(MovingObjects::getSize() / 2.f);
}

void Terrorist::setSpriteByDirection(){
    if(m_direction.x < 0){
        m_sprite.setTexture(TextureHolder::get(Textures::TerroristRunLeft));
    }else{
        m_sprite.setTexture(TextureHolder::get(Textures::TerroristRunRight));
    }
}

void Terrorist::update(const sf::Time& dt) {
    if (m_isJailed)
        return;

    // need to update timer even if dragged
    m_timer.update(dt);
    m_animation.update(dt);

    // update movement
    if (!m_isDragged)
        MovingObjects::update(dt);

    if(m_direction.x < 0)
        m_footStep.setPosition(sf::util::getGlobalBottomRight(*this) - sf::Vector2f(10, 25));
    else
        m_footStep.setPosition(sf::util::getGlobalBottomLeft(*this) - sf::Vector2f(10, 25));

	if(deffer++ % 3 == 0){
        TrailParticle p;
        p.m_color = m_color;
        m_footStep.addParticles(p, 1);
	}
    m_footStep.update(dt);

    if (!sf::FloatRect(-10.0, -10.0, m_winSize.x, m_winSize.y).contains(getPosition())){
        kill();
        // this cause the game state to update the amount of non jailed bomb which is wrong if just calling kill because game state not updated about it
        MessageBus::notify(MessageType::TerroristJailed); 
        LOGE << "bomb out";
    }
};

void Terrorist::handleEvent(const sf::Event& e) {
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

void Terrorist::handleCollision(Entity* e, const sf::Vector3f& manifold) {
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

void Terrorist::resolveCollision(Entity* e){
    auto jail = dynamic_cast<Jail*>(e);  // needed for getting jail's color.
    if (m_color != jail->getColor()) {
        m_timer.reset();  // calls kill and add explosion
    }
    else {
        MessageBus::notify(MessageType::TerroristJailed);
        jail->addTerrorist(this);
        arrest();
        playSound();
    }
}

void Terrorist::draw(sf::RenderTarget& win, sf::RenderStates states) const {
    MovingObjects::draw(win, states);
    sf::CircleShape rec;
    rec.setRadius(20);
    rec.setFillColor(m_color);
    win.draw(rec, getTransform());
    m_footStep.draw(win);
}

void Terrorist::playSound(){
    static sf::Sound m_sound{SoundBufferHolder::get(SoundEffect::Jailed)};
    m_sound.play();
}