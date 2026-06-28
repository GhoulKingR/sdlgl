#include <cstdio>
#include <cstdlib>
#include <sdlgl/sdlgl.hpp>

static SDL_Window* window;
static SDL_GLContext ctx;

int main() {
  // init sdlgl
  auto result = sdlgl::init(window, ctx, {.title = "SDL Examples"});
  if (!result) {
    fprintf(stderr, "%s\n", sdlgl::geterror());
    return EXIT_FAILURE;
  }

  // main loop
  while (!sdlgl::windowShouldClose()) {
    sdlgl::pollEvent(nullptr);
    SDL_GL_SwapWindow(window);
  }

  sdlgl::cleanup();
  return EXIT_SUCCESS;
}
