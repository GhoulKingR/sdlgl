#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

#include <cstdint>
#include <functional>

namespace sdlgl {
struct initOptions {
  const char* title = nullptr;  // title of the window
  int width = 800;
  int height = 600;
  bool vsync = false;
  uint64_t windowOptions = 0;             // SDL window creation options
  uint32_t initOptions = SDL_INIT_VIDEO;  // SDL init options
  uint8_t glMajorVersion = 3;             // OpenGL major version
  uint8_t glMinorVersion = 3;             // OpenGL minor version
};

bool init(SDL_Window*& window, SDL_GLContext& ctx, const initOptions&);
bool windowShouldClose();
void setWindowShouldClose(bool);
void pollEvent(std::function<void(SDL_Event&)>);
const char* geterror();
void cleanup();
}  // namespace sdlgl
