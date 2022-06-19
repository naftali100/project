// entity that can:
// move - transformable
// collide - see collision section
// draw  - see sfml section
// handle events - "
// be graph - TODO
// receive handlers to certain events
// some sort of id that can use to identify derived entities
#pragma once

#include <typeinfo>

#include "BaseEntity.h"
#include "CollisionTag.h"
#include "Colors.h"
#include "SfmlUtil.h"

class Entity : public sf::Transformable, public sf::Drawable, public BaseEntity {
public:
    virtual void handleEvent(const sf::Event&) override{
        // check event map
    };

    virtual void draw(sf::RenderTarget& target,
        sf::RenderStates states = sf::RenderStates::Default) const override {
        // debug draw
        // rectangle of global bound
        target.draw(sf::util::debugDraw(*this));
    }

    virtual void onEvent(const sf::Event::EventType& e, Func func) {
        // map<event, function>
    }

    /// collision handling

    // usage example
    // check by tag
    // if(e->getCollisionTag() == enemy){
    //     m_health --;
    // }
    virtual void handleCollision(Entity* e, const sf::Vector3f& manifold = {}){};
    void setCollisionTag(CollisionTag tag) {
        m_colTag = tag;
    };
    CollisionTag getCollisionTag() const {
        return m_colTag;
    };

    /// shape specific methods

    virtual void setSize(const sf::Vector2f& s) {
        m_size = s;
    }

    virtual void setSize(const float x, const float y) {
        m_size = {x, y};
    }

    virtual sf::Vector2f getSize() const {
        return m_size;
    }

    virtual sf::FloatRect getGlobalBounds() const {
        return sf::FloatRect(getPosition() - getOrigin(), m_size);
    }

    virtual std::string getType() const {
        return typeid(*this).name();
        // return std::type_info(typeid(*this));
    }

    virtual ~Entity() = default;

private:
    CollisionTag m_colTag = CollisionTag::none;  // by default collision disabled
    sf::Vector2f m_size;
    float m_gravity;  // ???
};
