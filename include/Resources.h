#pragma once
#ifndef resources_identifiers_h
#define resources_identifiers_h

#include "Assets.h"
#include "resource_holder.h"

// Forward declaration of SFML classes
namespace sf {
class Texture;
class Font;
class Shader;
class SoundBuffer;
}  // namespace sf

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

using TextureHolder = ResourceHolder<sf::Texture, Textures::ID>;
using FontHolder = ResourceHolder<sf::Font, Fonts::ID>;
using ShaderHolder = ResourceHolder<sf::Shader, Shaders::ID>;
using SoundBufferHolder = ResourceHolder<sf::SoundBuffer, SoundEffect::ID>;
using MusicHolder = ResourceHolder<sf::Music, Music::ID>;

#endif