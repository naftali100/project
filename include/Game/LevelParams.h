#ifndef __LEVELPARAMS_H__
#define __LEVELPARAMS_H__


//constants:
const int SPEED = 300;
const int BOMB_SPAWN_RATE = 3;
const int GIFT_SPAWN_RATE = 20;
const int COLORS = 2;
const float SCALE = 0;
const float TIME_OUT_TIMER = 10;
const float GIFT_TIME = 10;
const int BOMB_TIME = 10;
const int MAX_BOMB = 4;
const int BOMB_TO_SCORE = 3;

struct LevelParams {
    int m_speed = SPEED;
    int m_bombSpawnRate = BOMB_SPAWN_RATE;
    int m_giftSpawnRate = GIFT_SPAWN_RATE;
    int m_colors = COLORS;
    // sf::Vector2f m_size {5,5};
    float m_scale = SCALE;
    float m_timeoutTimer = TIME_OUT_TIMER;
    float m_giftTime = GIFT_TIME;
    int m_bombTime = BOMB_TIME;
    int m_maxTerrorist = MAX_BOMB;
    int m_bombToScore = BOMB_TO_SCORE;

    bool operator==(const LevelParams& other) const {
        return other.m_bombTime == m_bombTime &&
        other.m_colors == m_colors &&
        other.m_giftTime == m_giftTime &&
        other.m_scale == m_scale &&
        other.m_bombSpawnRate == m_bombSpawnRate &&
        other.m_giftSpawnRate == m_giftSpawnRate &&
        other.m_speed == m_speed
        && other.m_maxTerrorist == m_maxTerrorist
        && other.m_bombToScore == m_bombToScore
        ;
    }
    // bool operator!=(const LevelParams& other) const {
    //     return !(*this == other);
    // }
};

#endif  // __LEVELPARAMS_H__
