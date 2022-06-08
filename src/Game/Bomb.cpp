#include "Game/Bomb.h"

#include <mutex>

#include "Game/Jail.h"
#include "MessageBus.h"
#include "Random.h"
#include "SfmlUtil.h"

Bomb::Bomb(std::vector<std::unique_ptr<Explosion>>& explosions, const LevelParams& p) : m_explosions(explosions) {
    setCollisionTag(CollisionTag::bomb);
    m_sprite.setTexture(TextureHolder::get(Textures::Bomb));
    float scale = 2;
    m_sprite.scale(1 / scale, 1 / scale);
    MovingObjects::setSize(sf::Vector2f(TextureHolder::get(Textures::Bomb).getSize() / unsigned(scale)));
    setOrigin(MovingObjects::getSize() / 2.f);
    m_timer.set(
        [this]() {
            MessageBus::notify(MessageType::BombTimedout);
            kill();
            m_explosions.push_back(std::make_unique<Explosion>(getPosition()));
        },
        p.m_bombTime);

    initFromLevelParam(p);
    registerMessageHandler();
}

void Bomb::initFromLevelParam(const LevelParams& p, bool initColor) {
    MovingObjects::setSpeed(p.m_speed);
    // TODO: random color?
    // Colors::STD_COLORS[Random::rnd(0, p.m_colors)];
    // do it only once when init not when params is changing
    if (initColor)
        m_color = Colors::STD_COLORS[Random::rnd(0, p.m_colors)];
}

void Bomb::registerMessageHandler() {
    m_sub = MessageBus::subscribe<LevelParams*>(MessageType::LevelParamsUpdated, [this](LevelParams const* p) {
        initFromLevelParam(*p, false);
        // MovingObjects::setSpeed(p->m_speed);
    });
}

void Bomb::update(const sf::Time& dt) {
    if (m_isJailed)
        return;

    // need to update timer even if dragged
    m_timer.update(dt);

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
    if (m_isJailed)
        return;
    if (e->getCollisionTag() == CollisionTag::jail) {
        sf::FloatRect tempRect;
        // if intersects, and the whole entity is inside the jail
        if (getGlobalBounds().intersects(e->getGlobalBounds(), tempRect) && tempRect.width == getGlobalBounds().width &&
            tempRect.height == getGlobalBounds().height) {
            auto jail = dynamic_cast<Jail*>(e);  // needed for getting jail's color. TODO: can we avoid this?
            if (m_color != jail->getColor()) {
                // make the bomb to timeout and "explode"
                m_timer.reset();
            }
            else {
                MessageBus::notify(MessageType::BombJailed);
                jail->addBomb(this);
                arrest();
                // m_nonJailedBombCounter --;
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
    rec.setOrigin(sf::util::getGlobalCenter(rec));
    rec.setPosition(getPosition());
    // rec.setPosition(sf::util::getGlobalTopLeft(*this));
    rec.setFillColor(m_color);
    win.draw(rec);
}

Bomb::~Bomb() {
    m_sub();
}
