#ifndef __MESSAGEBUS_H__
#define __MESSAGEBUS_H__
#pragma once

enum class MessageType
{
    MessageId,
    BombJailed,
    BombTimedout,
    BombRemoveFromVector
};

class MessageBus {
public:
    /// register

    // just basic ping to all subscribers. needed?
    static void addReceiver(std::function<void()> f) {
        getInstance().m_subscribers.push_back(f);
    };

    static void addReceiver(MessageType type, std::function<void()> func) {
        getInstance().m_subscribersWithType.push_back(std::pair{type, func});
    };

    template <typename T>
    static void addReceiver(std::function<void(const T&)> func) {
        m_subscribersWithArg<T>.push_back(func);
    };

    template <typename T>
    static void addReceiver(MessageType type, std::function<void(const T&)> func) {
        m_subscribersWithTypeAndArg<T>.push_back(std::pair{type, func});
    };

    /// send events

    static void notify() {
        for (auto& i : getInstance().m_subscribers) { i(); }
    }

    static void notify(MessageType t) {
        for (auto& i : getInstance().m_subscribersWithType) {
            if (i.first == t)
                i.second();
        }
    }

    template <typename T>
    static void notify(const T& t) {
        for (auto i : m_subscribersWithArg<T>) {
            i(t); 
        }
    }

    template <typename T>
    static void notify(MessageType type, const T& param) {
        for (auto i : m_subscribersWithTypeAndArg<T>) {
            if (i.first == type)
                i.second(param);
        }
    }

private:
    static MessageBus& getInstance() {
        static MessageBus instance;
        return instance;
    }

    MessageBus() = default;
    MessageBus(const MessageBus&) = delete;

    std::vector<std::function<void()>> m_subscribers;
    std::vector<std::pair<MessageType, std::function<void()>>> m_subscribersWithType;

    template <typename T>
    inline static std::vector<std::function<void(const T&)>> m_subscribersWithArg;

    template <typename T>
    inline static std::vector<std::pair<MessageType, std::function<void(const T&)>>> m_subscribersWithTypeAndArg;
};
#endif  // __MESSAGEBUS_H__
