#ifndef __LEVELPARAMS_H__
#define __LEVELPARAMS_H__

struct LevelParams
{
    float m_speed;
    int m_spawnRate; // MAYBE: spread across time and not spawn in fixed intervals
    int m_colors;
    // sf::Vector2f m_size {5,5};
    float m_scale;
    float m_timer;  
    float m_giftTime;
};

#endif // __LEVELPARAMS_H__