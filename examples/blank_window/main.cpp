#include <cstdio>
#include <cstdlib>
#include <sdlgl/sdlgl.hpp>

static SDL_Window* window;
static SDL_GLContext ctx;

int main() {
  auto result = sdlgl::init(window, ctx, {.title = "SDL Examples"});
  if (!result) {
    fprintf(stderr, "%s\n", sdlgl::geterror());
    return EXIT_FAILURE;
  }

  while (!sdlgl::windowShouldClose()) {
    sdlgl::pollEvent(nullptr);
    SDL_GL_SwapWindow(window);
  }
}
