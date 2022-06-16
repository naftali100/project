#include "Game/Bomb.h"

#include <mutex>

#include "Game/Jail.h"
#include "Random.h"
#include "SfmlUtil.h"

Bomb::Bomb(std::vector<std::unique_ptr<Explosion>>& explosions, const LevelParams& p, const sf::Vector2f& pos, const sf::Vector2f& dir) : m_explosions(explosions) {
    setPosition(pos);
    setDirection(dir);
    initSprite();
    configLevelParam(p);
    registerMessageHandler();
    // color here because it's should done only once
    m_color = Colors::STD_COLORS[Random::rnd(0, p.m_colors + 1)];

    setCollisionTag(CollisionTag::bomb);

    m_timer.set(
        [this]() {
            MessageBus::notify(MessageType::BombTimedout);
            kill();
            m_explosions.push_back(std::make_unique<Explosion>(getPosition()));
        },
        p.m_bombTime);
}

void Bomb::initSprite() {
    m_sprite.setTexture(TextureHolder::get(Textures::Terrorist));
    m_animation.initFramesWithFixedSize(m_sprite.getTexture()->getSize(), 3, 4, 0.08f);
    m_animation.setFrame(0);

    float scale = 2;
    m_sprite.scale(sf::Vector2f(1, 1) / scale);
    MovingObjects::setSize(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height);
    setOrigin(MovingObjects::getSize() / 2.f);
}

void Bomb::configLevelParam(const LevelParams& p) {
    MovingObjects::setSpeed(p.m_speed);
}

void Bomb::registerMessageHandler() {
    m_sub = MessageBus::subscribe<LevelParams*>(MessageType::LevelParamsUpdated, [this](LevelParams const* p) {
        configLevelParam(*p);
    });
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
            auto jail = dynamic_cast<Jail*>(e);  // needed for getting jail's color. TODO: can we avoid this?
            if (m_color != jail->getColor()) {
                m_timer.reset();  // calls kill and add explosion
            }
            else {
                MessageBus::notify(MessageType::BombJailed);
                jail->addBomb(this);
                arrest();
            }
        }
    }

    if (!m_isDragged)
        MovingObjects::handleCollision(e, manifold);
}

void Bomb::draw(sf::RenderTarget& win, sf::RenderStates states) const {
    MovingObjects::draw(win, states);
    sf::CircleShape rec;
    rec.setRadius(20);
    rec.setFillColor(m_color);
    win.draw(rec, getTransform());
}

Bomb::~Bomb() {
    m_sub();
}
