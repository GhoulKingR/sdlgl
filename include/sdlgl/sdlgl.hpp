#pragma once

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include <cstdint>
#include <functional>

namespace sdlgl {
struct initOptions {
  const char* title = nullptr;  // title of the window (Required)
  int width = 800;              // Initial window height (Default 800)
  int height = 600;             // Initial window width (Default 600)
  bool vsync = false;           // enable/disable vsync (Default false)

  // SDL window creation options (SDL_WINDOW_OPENGL already set internally)
  uint64_t windowOptions = 0;

  // SDL init options (SDL_INIT_VIDEO) already set internally)
  uint32_t initOptions = 0;

  uint8_t glMajorVersion = 3;  // OpenGL major version (Default 3)
  uint8_t glMinorVersion = 3;  // OpenGL minor version (Default 3)
  int glProfile = SDL_GL_CONTEXT_PROFILE_CORE;  // OpenGL profile (Default
                                                // SDL_GL_CONTEXT_PROFILE_CORE)
  struct {
    bool enable = false;   // Enable multisampling (Default false)
    int samples = 4;       // number of samples in multisampler (Default 4)
    int buffers = 1;       // number of multisample buffers (Default 1)
  } multisampler;          // multisampler options
  bool depthTest = false;  // Enable depth test (Default false)
};

bool init(SDL_Window*& window, SDL_GLContext& ctx, const initOptions&);
bool windowShouldClose();
void setWindowShouldClose(bool);
void pollEvent(std::function<void(SDL_Event&)>);
void getViewport(int& w, int& h);
const char* geterror();
void cleanup();
}  // namespace sdlgl
