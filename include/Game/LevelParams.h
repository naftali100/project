#ifndef __LEVELPARAMS_H__
#define __LEVELPARAMS_H__

struct LevelParams
{
    float m_speed = 300;
    float m_spawnRate = 3; // MAYBE: spread across time and not spawn in fixed intervals
    int m_colors = 2;
    // sf::Vector2f m_size {5,5};
    float m_scale = 0;
    float m_timeoutTimer = 10;
    float m_giftTime = 10;
    float m_bombTime = 10;
    int m_maxBomb = 4;
    int m_bombToScore = 3;

    bool operator==(const LevelParams& other) const {
        return other.m_bombTime == m_bombTime &&
        other.m_colors == m_colors &&
        other.m_giftTime == m_giftTime &&
        other.m_scale == m_scale &&
        other.m_spawnRate == m_spawnRate &&
        other.m_speed == m_speed
        && other.m_maxBomb == m_maxBomb
        && other.m_bombToScore == m_bombToScore
        ;
    }
    // bool operator!=(const LevelParams& other) const {
    //     return !(*this == other);
    // }
};

#endif // __LEVELPARAMS_H__