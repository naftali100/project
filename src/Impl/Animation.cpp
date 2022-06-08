#include "Animation.h"

Animation::Animation(sf::Sprite& target) : m_target(target) {
    progress = totalLength = 0.0;
}

Animation::Animation(const Textures::ID& t, const sf::Vector2f& pos) : m_target(m_spriteStorage) {
    m_spriteStorage.setTexture(TextureHolder::get(t));
    m_spriteStorage.setPosition(pos);
}

void Animation::addFrame(Frame&& frame) {
    totalLength += frame.duration;
    m_frames.push_back(std::move(frame));
}

void Animation::setDuration(float d) {
    m_duration = d;
}

void Animation::update(const sf::Time& elapsed) {
    LOGV << "start";
    if (m_duration > 0 && progress >= m_duration) {
        m_target.setTextureRect({0, 0, 0, 0});
        return;
    }
    progress += elapsed.asSeconds();
    auto p = std::fmod(progress, totalLength);
    for (size_t i = 0; i < m_frames.size(); i++) {
        p -= m_frames.at(i).duration;

        // if we have progressed OR if we're on the last frame, apply and stop.
        if (p > 0.0 && &(m_frames.at(i)) == &m_frames.back()) {
            i = 0;     // start over from the beginning
            continue;  // break off the loop and start where i is
        }
        if (p <= 0.0) {
            m_target.setTextureRect(m_frames.at(i).rect);
            break;  // we found our frame
        }
    }
    LOGV << "finish";
}

void Animation::initFramesWithFixedSize(const sf::Vector2u& textureSize, int atlasRows, int atlasCols,
                                        float frameTime) {
    int FrameXSize = textureSize.x / atlasCols;
    int FrameYSize = textureSize.y / atlasRows;

    for (int i = 0; i < atlasRows; i++) {
        for (int j = 0; j < atlasCols; j++) {
            addFrame({sf::IntRect(j * FrameXSize, i * FrameYSize, FrameXSize, FrameYSize), frameTime});
        }
    }
}

void Animation::setFrame(int index) {
    m_target.setTextureRect(m_frames.at(index).rect);
}
