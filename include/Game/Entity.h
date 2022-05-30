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
#include "Colors.h"

enum class CollisionTag
{
    player,
    wall,
    jail,
    enemy,
    bomb,
    none
};

class Entity : public sf::Transformable, public sf::Drawable {
public:
    Entity() = default;

    /// basic sfml object methods

    virtual void update(const sf::Time& dt) = 0;
    virtual void handleEvent(const sf::Event& e){
        // check in events map
    };
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override {
        // debug draw
        // rectangle of global bound
        sf::RectangleShape bound{getSize()};
        bound.setPosition(getGlobalBounds().left, getGlobalBounds().top);
        bound.setFillColor(Colors::Transparent);
        bound.setOutlineColor(Colors::Red);
        bound.setOutlineThickness(2);

        target.draw(bound);
    }

    virtual void onEvent(const sf::Event::EventType& e, std::function<void()> func) {
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
        // getTransform().transformRect()
        return sf::FloatRect(getPosition(), m_size);
    }

    virtual std::string getType() const {
        return typeid(*this).name();
        // return std::type_info(typeid(*this));
    }

private:
    CollisionTag m_colTag = CollisionTag::none;  // by default collision disabled
    sf::Vector2f m_size;
    float m_gravity;  // ???
};
