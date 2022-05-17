#pragma once
#ifndef RANDOM_H
#define RANDOM_H

#include <random>

namespace Random {
    inline std::random_device device_;
    inline std::default_random_engine engine_(device_());

    inline int rnd(int a, int b) {
        std::uniform_int_distribution<int> uni_dist(a, b);
        return uni_dist(engine_);
    };
    inline double rnd(double a, double b) {
        std::uniform_real_distribution<double> uni_dist(a, b);
        return uni_dist(engine_);
    };
};

#endif