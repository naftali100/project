#pragma once
#ifndef LOG_H
#define LOG_H

// convenience file

#include "SfmlUtil.h"
#include <utility>
// #define PLOG_OMIT_LOG_DEFINES - disable LOG* prefix

template <typename T>
concept PrintableVec = requires(T t) {
    t.x;
    t.y;
};

template <typename T>
concept PrintableShape = requires(T t) {
    t.getPosition();
    t.getSize();
    sf::util::getGlobalTopLeft(t);
};

namespace plog {
template <typename T>
Record& operator<<(Record& record, const sf::Rect<T>& v) {
    record << v.top << ", " << v.left << " : " << v.width << ", " << v.height
           << " (low right corner: " << v.left + v.width << ", " << v.top + v.height << ")";
    return record;
}

template <PrintableShape S>
Record& operator<<(Record& record, const S& s) {
    record << s.getPosition().x << ", " << s.getPosition().y << " : " << s.getSize().x << ", " << s.getSize().y
           << " (low r corner " << sf::util::getGlobalBottomRight(s) << ")";
    return record;
}

template <PrintableVec V>
Record& operator<<(Record& record, const V& v) {
    record << v.x << ", " << v.y;
    return record;
}
}  // namespace plog


// HACK ... probably not thread safe (maybe replace with std::once)
#define PLOG_ONCE(severity) \
    static int once = 2;    \
    if (once > 0) once--;   \
    PLOG_IF(severity, once > 0)

#define LOGV_ONCE PLOG_ONCE(plog::verbose)
#define LOGD_ONCE PLOG_ONCE(plog::debug)
#define LOGI_ONCE PLOG_ONCE(plog::info)
#define LOGW_ONCE PLOG_ONCE(plog::warning)
#define LOGE_ONCE PLOG_ONCE(plog::error)
#define LOGF_ONCE PLOG_ONCE(plog::fatal)
#define LOGN_ONCE PLOG_ONCE(plog::none)

#endif