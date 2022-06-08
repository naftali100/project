#ifndef __MESSAGETYPES_H__
#define __MESSAGETYPES_H__
#pragma once

enum class MessageType
{
    //add here non-Gift enums
    MessageId,
    BombJailed,
    BombTimedout,
    BombRemoveFromVector,
    LevelParamsUpdated,
    // add here gift enums only
    ScoreGift,
    RemoveTerroristsGift,
    LiveGift,
    FreeTerroristsGift,
    //don't add here non-gift enums!
    Count
};

#endif // __MESSAGETYPES_H__