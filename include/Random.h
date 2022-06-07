#pragma once
#ifndef RANDOM_H
#define RANDOM_H

#include <random>

namespace Random {
// TODO: check if now that we have c++20, if inline variables working
std::random_device device_;
std::default_random_engine engine_(device_());

inline int rnd(int a, int b) {
    std::uniform_int_distribution<int> uni_dist(a, b);
    return uni_dist(engine_);
};
template<typename T>
inline T rnd(T a, T b) {
    std::uniform_real_distribution<T> uni_dist(a, b);
    return uni_dist(engine_);
};
};  // namespace Random

#endif