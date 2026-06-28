#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl3.h>

#include <cstdio>
#include <cstdlib>
#include <sdlgl/sdlgl.hpp>

#include "SDL3/SDL_video.h"

static SDL_Window* window;
static SDL_GLContext ctx;

int main() {
  // init sdlgl
  auto result = sdlgl::init(
      window, ctx,
      {.title = "SDL Examples", .windowOptions = SDL_WINDOW_RESIZABLE});
  if (!result) {
    fprintf(stderr, "%s\n", sdlgl::geterror());
    return EXIT_FAILURE;
  }

  // init imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  auto& io = ImGui::GetIO();
  io.ConfigWindowsMoveFromTitleBarOnly = true;
  ImGui_ImplSDL3_InitForOpenGL(window, ctx);
  ImGui_ImplOpenGL3_Init();

  // main loop
  while (!sdlgl::windowShouldClose()) {
    sdlgl::pollEvent([](auto& event) { ImGui_ImplSDL3_ProcessEvent(&event); });
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();
    ImGui::ShowMetricsWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
  }

  // cleanup ImGui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  sdlgl::cleanup();
  return EXIT_SUCCESS;
}
