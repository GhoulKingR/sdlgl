#include <SDL3/SDL.h>
#include <glad/glad.h>

#include <cassert>
#include <cstdlib>
#include <sdlgl/sdlgl.hpp>
#include <string>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"

static std::string errorText = "";
static int width;
static int height;
static SDL_Window** windowPtr = nullptr;
static SDL_GLContext* ctxPtr = nullptr;

bool sdlgl::init(SDL_Window*& window, SDL_GLContext& ctx,
                 const initOptions& options) {
  width = options.width;
  height = options.height;
  assert(options.title != nullptr && "Title cannot be null");

  if (!SDL_Init(options.initOptions)) {
    errorText = "Error :: Failed to initialize SDL: ";
    errorText += SDL_GetError();
    return false;
  }

#if __APPLE__
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, options.glProfile);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, options.glMajorVersion);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, options.glMinorVersion);

  if (options.multisampler.enable) {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,
                        options.multisampler.buffers);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,
                        options.multisampler.samples);
  }
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  window = SDL_CreateWindow(options.title, width, height,
                            SDL_WINDOW_OPENGL | options.windowOptions);
  windowPtr = &window;

  if (window == nullptr) {
    errorText = "Error :: Failed to initialize SDl window: ";
    errorText += SDL_GetError();
    SDL_Quit();
    return false;
  }

  ctx = SDL_GL_CreateContext(window);
  ctxPtr = &ctx;
  if (ctx == nullptr) {
    errorText = "Error :: Failed to create GL context: {}";
    errorText += SDL_GetError();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }

  SDL_GL_MakeCurrent(window, ctx);
  SDL_GL_SetSwapInterval(options.vsync);
  SDL_ShowWindow(window);

  if (!gladLoadGLLoader(
          reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
    errorText = "Error :: Failed to initialize GLAD";
    SDL_GL_DestroyContext(ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }

  glViewport(0, 0, width, height);
  if (options.multisampler.enable) {
    glEnable(GL_MULTISAMPLE);
  }
  return true;
}

static bool running = true;
bool sdlgl::windowShouldClose() { return !running; }
void sdlgl::setWindowShouldClose(bool val) { running = !val; }

void sdlgl::pollEvent(std::function<void(SDL_Event&)> callback) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      running = false;
    } else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
               event.window.windowID == SDL_GetWindowID(*windowPtr)) {
      running = false;
    } else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
      SDL_GetWindowSizeInPixels(*windowPtr, &width, &height);
      glViewport(0, 0, width, height);
    }

    if (callback != nullptr) callback(event);
  }
}

void sdlgl::getViewport(int& w, int& h) {
  w = width;
  h = height;
}
const char* sdlgl::geterror() { return errorText.c_str(); }
void sdlgl::cleanup() {
  if (ctxPtr != nullptr) SDL_GL_DestroyContext(*ctxPtr);
  if (windowPtr != nullptr) SDL_DestroyWindow(*windowPtr);
  SDL_Quit();
}
