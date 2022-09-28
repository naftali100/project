#ifndef __LEVELPARAMS_H__
#define __LEVELPARAMS_H__


//constants:
const int SPEED = 300;
const int TERRORIST_SPAWN_RATE = 3;
const int GIFT_SPAWN_RATE = 20;
const int COLORS = 2;
const float SCALE = 0;
const float TIME_OUT_TIMER = 10;
const float GIFT_TIME = 10;
const int TERRORIST_TIME = 10;
const int MAX_TERRORIST = 4;
const int TERRORIST_TO_SCORE = 3;

struct LevelParams {
    int m_speed = SPEED;
    int m_terroristSpawnRate = TERRORIST_SPAWN_RATE;
    int m_giftSpawnRate = GIFT_SPAWN_RATE;
    int m_colors = COLORS;
    // sf::Vector2f m_size {5,5};
    float m_scale = SCALE;
    float m_timeoutTimer = TIME_OUT_TIMER;
    float m_giftTime = GIFT_TIME;
    int m_terroristTime = TERRORIST_TIME;
    int m_maxTerrorist = MAX_TERRORIST;
    int m_terroristToScore = TERRORIST_TO_SCORE;

    bool operator==(const LevelParams& other) const {
        return other.m_terroristTime == m_terroristTime &&
        other.m_colors == m_colors &&
        other.m_giftTime == m_giftTime &&
        other.m_scale == m_scale &&
        other.m_terroristSpawnRate == m_terroristSpawnRate &&
        other.m_giftSpawnRate == m_giftSpawnRate &&
        other.m_speed == m_speed
        && other.m_maxTerrorist == m_maxTerrorist
        && other.m_terroristToScore == m_terroristToScore
        ;
    }
    // bool operator!=(const LevelParams& other) const {
    //     return !(*this == other);
    // }
};

#endif  // __LEVELPARAMS_H__
