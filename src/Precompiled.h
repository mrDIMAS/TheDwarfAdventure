/**********************************************************************
 * The Dwarf Adventure - 2D adventure RPG with destructible world     *
 *                                                                    *
 * Copyright(C) 2016-2019 Dmitry Stepanov a.k.a mrDIMAS a.k.a v1al    *
 *                                                                    *
 * This program is free software: you can redistribute it and/or      *
 * modify it under the terms of the GNU General Public License as     *
 * published by the Free Software Foundation, either version 3 of     *
 * the License, or (at your option) any later version.                *
 *                                                                    *
 * This program is distributed in the hope that it will be useful,    *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * along with this program. If not, see https://www.gnu.org/licenses/ *
 *                                                                    *
 **********************************************************************/

#ifdef _MSC_VER
#  pragma warning(disable : 4503)
#endif

// Useful macros
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#define GLEW_STATIC
#define FIXED_FPS (60.0f)
#define PER_SECOND(x) ((x) * (1.0f / FIXED_FPS))
#define SECONDS(s) (static_cast<int>((s) * FIXED_FPS))
#define BIT(n) static_cast<int>(1 << (n))
#define THROW_IF_EXPIRED(ptr)                                                                      \
  if (ptr.expired()) {                                                                             \
    throw Exception(StringBuilder()                                                            \
                        << __FILE__ << " at line " << __LINE__ << " in " << __func__ << " - "      \
                        << #ptr << " is unexpected expired weak_ptr!");                            \
  }
#define THROW_IF_NULL(ptr)                                                                         \
  if (ptr == nullptr) {                                                                            \
    throw Exception(StringBuilder()                                                            \
                        << __FILE__ << " at line " << __LINE__ << " in " << __func__ << " - "      \
                        << #ptr << " is unexpected null pointer!");                                \
  }
#define UNUSED(x) (void)(x)
#define LENGTH_OF(a) ((int)(sizeof((a)) / sizeof(*(a))))

#ifdef DBG_ECHO_ENABLED
#define DBG_ECHO() Log() << __func__ << LineEnd;
#else
#define DBG_ECHO()
#endif

#define ASSERT(expression, message)  if(!(expression)) throw Exception(message)

#ifdef _MSC_VER
#  include <experimental/filesystem>
#else
#  include <filesystem>
#endif
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <array>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <random>
#include <fstream>
#include <stack>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <cmath>
#include <condition_variable>
#include <cstdint>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio.hpp>

using namespace std;
#ifdef _MSC_VER
using namespace experimental;
#endif

#include "Exception.h"
#include "StringBuilder.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

using Vec2f = sf::Vector2f;
using Vec2i = sf::Vector2i;

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/error/en.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"

#include "json.hpp"

class Serializer;
void Serialize(shared_ptr<sf::SoundBuffer> &buf, Serializer &sav);

#include "Serializer.h"
#include "Log.h"
#include "MathLib.h"
#include "Enum.h"

#include "Guid.h"

// Do not pollute global namespace
#define MINIZ_NO_ZLIB_COMPATIBLE_NAMES
#include "miniz/miniz.h"

void LoadJSON(const std::string & filename, rapidjson::Document & doc);
string RoundAndConvert(float n, unsigned int decimalPlaces);
void RaiseMessageAndDie(const string & message) noexcept;
void ReportError(const string & message);
string ColorToStr(sf::Color clr);

// Forward declarations
class Game;
class Serializer;
class Canvas;
class Window;
class Label;
class Scene;
class Button;
class SceneNode;
class Sprite;
class ParticleSystem;
class Location;
class Actor;
class Door;
class AnimationTrack;
class Table;
class Player;
class Texture;
class Item;
class NumericUpDown;
class Tooltip;
class Inventory;
class Layout;
class HUD;
class Image;
class Frame;
class PolyLine;
class ScrollableArea;
class Animation;
class SlideList;
class Slot;
class Room;
class Sound;
class LineSet;
class MapWindow;
class Light;
class Workbench;
class Router;
class Terrain;
struct TerrainBrush;

// This single nasty global variable is entry point for entire game
// by now this is the only one global variable in the whole project
extern Game *game;
