#pragma once

#include <glad/glad.h>
#include <SDL3/SDL.h>

#include <cstdint>
#include <functional>

namespace sdlgl {
struct initOptions {
  const char* title = nullptr;                  // title of the window
  int width = 800;                              // Initial window height
  int height = 600;                             // Initial window width
  bool vsync = false;                           // enable/disable vsync
  uint64_t windowOptions = 0;                   // SDL window creation options
  uint32_t initOptions = SDL_INIT_VIDEO;        // SDL init options
  uint8_t glMajorVersion = 3;                   // OpenGL major version
  uint8_t glMinorVersion = 3;                   // OpenGL minor version
  int glProfile = SDL_GL_CONTEXT_PROFILE_CORE;  // OpenGL profile
  struct {
    bool enable = false;   // Enable multisampling
    int samples = 4;       // number of samples in multisampler
    int buffers = 1;       // number of multisample buffers
  } multisampler;          // multisampler options
  bool depthTest = false;  // Enable depth test
};

bool init(SDL_Window*& window, SDL_GLContext& ctx, const initOptions&);
bool windowShouldClose();
void setWindowShouldClose(bool);
void pollEvent(std::function<void(SDL_Event&)>);
void getViewport(int& w, int& h);
const char* geterror();
void cleanup();
}  // namespace sdlgl
