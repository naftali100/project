#pragma once
#ifndef COMMAND_H
#define COMMAND_H

#include <functional>
#include <iostream>

#include "Game.h"

// maybe: add paramer pack to constructor

template <typename T>
class Command {
public:
    typedef void (T::*Action)(/*Args...*/);
    Command(T* object, Action action) {
        // this is lambda for possible support for params in future
        m_callback = [object, action]() { (object->*action)(); };
    }
    // template <typename... Args>
    void execute(/*Args...*/) {
        m_callback(/*Args...args*/);
    }

private:
    std::function<void()> m_callback;
};

#endif