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

## Building examples

To build any of the examples in the [examples](examples) folder, run these commands:
```bash
mkdir build
cd build
cmake .. -DSDLGL_BUILD_EXAMPLES=ON -DCOLORED_SQUARE=ON -G Ninja
ninja
```

If you prefer to use make instead use these instead:
```bash
mkdir build
cd build
cmake .. -DSDLGL_BUILD_EXAMPLES=ON -DCOLORED_SQUARE=ON
make
```

This example runs the [colored square](examples/colored_square) example. You can check the list of available commands in the [example cmake file](examples/CMakeLists.txt). and replace `COLORED_SQUARE` in the cmake build command.

To run the built example run these comands:
```bash
cd examples/colored_square      # from the build directory
./colored_square
```

## Contributing

sdlgl currently contains the things I use regularly in my SDL and OpenGL applications. You may not find it to have all the features you need. So if there's any feature you wish to add, feel free to send a PR with the updates.
