#ifndef __MESSAGEBUS_H__
#define __MESSAGEBUS_H__
#pragma once

#include "MessageTypes.h"

class MessageBus {
public:
    template <typename T>
    using FuncT = std::function<void(const T&)>;

    /// subscribe

    // just basic ping to all subscribers. needed?
    static Func subscribe(const Func& f) {
        getInstance().m_subscribers.push_back({++getInstance().m_id, f});
        return [id = getInstance().m_id]() { unsubscribe(id); };
    };

    static Func subscribe(MessageType type, const Func& func) {
        getInstance().m_subscribersWithType.push_back(std::pair(++getInstance().m_id,
            std::pair(type, func)));
        return [id = getInstance().m_id]() { unsubscribe(id); };
    };

    template <typename T>
    static Func subscribe(const std::function<void(const T&)>& func) {
        m_subscribersWithArg<T>.push_back({++getInstance().m_id, func});
        return [id = getInstance().m_id]() { unsubscribe<T>(id); };
    };

    template <typename T>
    static Func subscribe(MessageType type, const FuncT<T>& func) {
        m_subscribersWithTypeAndArg<T>.push_back(std::pair(++getInstance().m_id,
            std::pair(type, func)));
        return [id = getInstance().m_id]() { unsubscribe<T>(id); };
    };

    /// unsubscribe

    static void unsubscribe(unsigned long int id) {
        std::erase_if(getInstance().m_subscribers, [id](auto i)
            { return i.first == id; });
        std::erase_if(getInstance().m_subscribersWithType,
            [id](auto i) { return i.first == id; });
    }

    template <typename T>
    static void unsubscribe(unsigned long int id) {
        std::erase_if(m_subscribersWithArg<T>, [id](auto i) { return i.first == id; });
        std::erase_if(m_subscribersWithTypeAndArg<T>, [id](auto i)
            { return i.first == id; });
    }

    /// send events

    static void notify() {
        LOGV;
        for (auto& i : getInstance().m_subscribers) { i.second(); }
        LOGV;
    }

    static void notify(MessageType t) {
        LOGV;
        for (auto& i : getInstance().m_subscribersWithType) {
            if (i.second.first == t)
                if (i.second.second)
                    i.second.second();
        }
        LOGV;
    }

    template <typename T>
    static void notify(const T& t) {
        for (auto i : m_subscribersWithArg<T>) {
            if (i.second)
                i.second(t);
        }
    }

    template <typename T>
    static void notify(MessageType type, const T& param) {
        LOGV;
        for (auto i : m_subscribersWithTypeAndArg<T>) {
            if (i.second.first == type)
                if (i.second.second)
                    i.second.second(param);
        }
        LOGV;
    }

private:
    static MessageBus& getInstance() {
        static MessageBus instance;
        return instance;
    }

    MessageBus() = default;
    MessageBus(const MessageBus&) = delete;

    std::vector<std::pair<int, Func>> m_subscribers;
    std::vector<std::pair<int, std::pair<MessageType, Func>>> m_subscribersWithType;

    // MAYBE: make this class templated and this two will be class members
    template <typename T>
    inline static std::vector<std::pair<int, std::function<void(const T&)>>> m_subscribersWithArg;

    template <typename T>
    inline static std::vector<std::pair<int, std::pair<MessageType,
        std::function<void(const T&)>>>> m_subscribersWithTypeAndArg;

    unsigned long int m_id = 0;
};
#endif  // __MESSAGEBUS_H__
