#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

struct Frame {
    sf::IntRect rect;
    double duration;  // in seconds
};

class Animation {
public:
    Animation(sf::Sprite &target);
    void initFramesWithFixedSize(const sf::Vector2u& textureSize, int atlasRows, int atlasCols, float frameTime);
    void addFrame(Frame &&frame);
    void setFrame(int index);
    void update(double elapsed);
    const double getLength() const {
        return totalLength;
    }

private:
    std::vector<Frame> m_frames;
    double totalLength;
    double progress;
    sf::Sprite &target;
};

#endif