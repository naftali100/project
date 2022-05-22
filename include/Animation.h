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
    // virtual ~Animation();
    void addFrame(Frame &&frame);
    void update(double elapsed);
    const double getLength() const {
        return totalLength;
    }

private:
    std::vector<Frame> frames;
    double totalLength;
    double progress;
    sf::Sprite &target;
};

#endif