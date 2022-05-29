#include "Animation.h"

Animation::Animation(sf::Sprite& target) : target(target) {
    progress = totalLength = 0.0;
}

void Animation::addFrame(Frame&& frame) {
    totalLength += frame.duration;
    m_frames.push_back(std::move(frame));
}

void Animation::update(const sf::Time& elapsed) {
    progress += elapsed.asSeconds();
    double p = progress;
    for (size_t i = 0; i < m_frames.size(); i++) {
        p -= m_frames[i].duration;

        // if we have progressed OR if we're on the last frame, apply and stop.
        if (p > 0.0 && &(m_frames[i]) == &m_frames.back()) {
            i = 0;     // start over from the beginning
            continue;  // break off the loop and start where i is
        }
        if (p <= 0.0) {
            target.setTextureRect(m_frames[i].rect);
            break;  // we found our frame
        }
    }
}

void Animation::initFramesWithFixedSize(const sf::Vector2u& textureSize, int atlasRows, int atlasCols, float frameTime) {
    int FrameXSize = textureSize.x / atlasCols;
    int FrameYSize = textureSize.y / atlasRows;

    for (int i = 0; i < atlasRows; i++) {
        for (int j = 0; j < atlasCols; j++) {
            addFrame({sf::IntRect(j * FrameXSize, i * FrameYSize, FrameXSize, FrameYSize), frameTime});
        }
    }
}


void Animation::setFrame(int index) {
    target.setTextureRect(m_frames.at(index).rect);
}
