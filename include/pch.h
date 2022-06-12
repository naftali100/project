#pragma once

#include <imgui-SFML.h>
#include <imgui.h>
#include <plog/Log.h>
#include <plog/Helpers/PrintVar.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <ranges>
#include <set>
#include <stack>
#include <utility>
#include <vector>

namespace rng = std::ranges;
// basic lambda with no params that return void
using Func = std::function<void()>;