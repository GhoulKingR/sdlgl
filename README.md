# sdlgl
sdl bootstrapper

## Usage

### submodule

To add sdlgl to your project via git submodule, follow these steps:
1. Add the repository to your project:
```bash
git submodule add https://github.com/GhoulKingR/sdlgl sdlgl
```
2. Add the following line to your `CMakeLists.txt`:
```cmake
add_subdirectory(sdlgl)

target_link_libraries(${PROJECT_NAME}
    sdlgl
)
```

After doing that, you'll be able to use `sdlgl` in your project.

## Example

Here's an example of how to use `sdlgl` (Taking the code from the [blank window](examples/blank_window) example):
```cpp
#include <cstdio>
#include <cstdlib>
#include <sdlgl/sdlgl.hpp>

// SDL window and context variables
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
```

You can also check out other examples of how to use sdlgl in the [examples](examples) directory.
