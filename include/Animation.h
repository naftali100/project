#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include "Resources.h"

struct Frame {
    sf::IntRect rect;
    double duration;  // in seconds
};

class Animation {
public:
    explicit Animation(sf::Sprite& target);
    Animation(const Textures::ID& t, const sf::Vector2f& pos);
    void initFramesWithFixedSize(const sf::Vector2u& textureSize, int atlasRows, int atlasCols, float frameTime);
    void addFrame(Frame&& frame);
    // select frame to show manually
    void setFrame(int index);
    // Set the length of time the animation will play for
    void setDuration(float time);
    void update(const sf::Time& elapsed);
    double getLength() const {
        return totalLength;
    }
    sf::Sprite& getTarget() {
        return m_target;
    }

private:
    std::vector<Frame> m_frames;
    double totalLength;
    double progress;
    sf::Sprite& m_target;
    sf::Sprite m_spriteStorage;
    sf::Vector2f m_pos;  // used only when using sprit storage
    float m_duration = -1;
};

#endif