#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

struct Frame {
    sf::IntRect rect;
    double duration;  // in seconds
};

class Animation {
public:
    explicit Animation(sf::Sprite &target);
    void initFramesWithFixedSize(const sf::Vector2u& textureSize, int atlasRows, int atlasCols, float frameTime);
    void addFrame(Frame &&frame);
    // select frame to show manually
    void setFrame(int index);
    // Set the length of time the animation will play for
    void setDuration(float time);
    void update(const sf::Time& elapsed);
    double getLength() const {
        return totalLength;
    }

private:
    std::vector<Frame> m_frames;
    double totalLength;
    double progress;
    sf::Sprite &target;
    float m_duration = -1;
};

#endif