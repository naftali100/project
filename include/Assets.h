#pragma once
#ifndef ASSETS_H
#define ASSETS_H

#include <string>
#include <map>

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
    Map
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
    Main
};
}  // namespace SoundEffect

namespace Music {
enum ID
{};
}  // namespace Music



#endif