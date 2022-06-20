#pragma once
#ifndef ASSETS_H
#define ASSETS_H

#include <map>
#include <string>

// maybe add struct with info ablout assets pathes and more

// ===============
// info about all assets
namespace States {
enum ID
{
    Main,
    Demo2,
    Resources,
    Collision,
    Gui,
    Particles,
    Animation,
    StatesCount
};
}  // namespace States

namespace Textures {
enum ID
{
    Player,
    FireAnimationSheet,
    Run,
    Map,
    SkyBackground,
    SoldierBackground,
    Bomb,
    Btn,
    Stars,
    Gift,
    Explosion,
    Jail,
    Door,
    TerroristRunLeft,
    TerroristRunRight,
    Gift2,
    Test,
    Smoke,
    Wall,
    //levels:
    level1,
    level2,
    level3,
    level4,
    level5,
    level1Background,
    level2Background,
    level3Background,
    level4Background,
    level5Background,
    MAX
};  // add textures here
}  // namespace Textures

namespace Shaders {
enum ID
{};
}  // namespace Shaders

namespace Fonts {
enum ID
{
    Main,
    Test
};
}  // namespace Fonts

namespace SoundEffect {
enum ID
{
    Main,
    Gift,
    Explosion,
    Lose,
    Jailed,
    Count
};
}  // namespace SoundEffect

namespace Music {
enum ID
{
    Background1,  // fun and suitable
    Background2,  // only dramatic, not fun
    Background3,  // fun, more music then drama
    Background4,  // funny :)
    Count
};
}  // namespace Music

#endif
