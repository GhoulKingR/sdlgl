#include <cstdio>
#include <cstdlib>
#include <sdlgl/sdlgl.hpp>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"

static SDL_Window* window;
static SDL_GLContext ctx;

int main() {
  auto result = sdlgl::init(window, ctx, {.title = "SDL Examples"});
  if (!result) {
    fprintf(stderr, "%s\n", sdlgl::geterror());
    return EXIT_FAILURE;
  }

  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      } else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
                 event.window.windowID == SDL_GetWindowID(window)) {
        running = false;
      }
    }

    SDL_GL_SwapWindow(window);
  }
}
