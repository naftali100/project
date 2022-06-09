#ifndef __HOLYTHREE_H__
#define __HOLYTHREE_H__

/// basic SFML object methods
class BaseEntity {
public:
    virtual void update(const sf::Time& dt) = 0;
    virtual void handleEvent(const sf::Event& e) = 0;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const = 0;

    virtual ~BaseEntity() = default;
};
#endif // __HOLYTHREE_H__