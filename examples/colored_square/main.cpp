#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <sdlgl/sdlgl.hpp>
#include <sdlgl/utility.hpp>

namespace shader = sdlgl::utility::shader;

static SDL_Window* window;
static SDL_GLContext ctx;
static const char* vertexShader = R"(#version 330 core
layout (location=0) in vec2 aPos;
layout (location=1) in vec3 aColor;

out vec3 outColor;

void main() {
    gl_Position = vec4(aPos, 0.0f, 1.0f);
    outColor = aColor;
}
)";

static const char* fragmentShader = R"(#version 330 core
in vec3 outColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(outColor, 1.0f);
}
)";

int main() {
  // init sdlgl
  auto result = sdlgl::init(window, ctx, {.title = "SDL Examples"});
  if (!result) {
    fprintf(stderr, "%s\n", sdlgl::geterror());
    return EXIT_FAILURE;
  }

  // create buffer objects
  uint32_t VBO = 0, VAO = 0, EBO = 0;
  uint32_t indexCount;

  // clang-format off
  float vertices[] = {
      // vertices    // colors
      -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
       0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
      -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
       0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
  };
  uint32_t indices[] = {2, 0, 1, 1, 3, 2};
  indexCount = sizeof(indices) / sizeof(indices[0]);
  // clang-format on

  // initialize buffer objects
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // Setup the vertex data for the shader input values
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);  // aPos
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);  // aColor
  glBindVertexArray(0);
  uint32_t shdr = shader::load(vertexShader, fragmentShader);
  if (shdr == 0) {
    fprintf(stderr, "%s\n", sdlgl::geterror());
    return EXIT_FAILURE;
  }

  // main loop
  while (!sdlgl::windowShouldClose()) {
    sdlgl::pollEvent(nullptr);
    shader::use(shdr);

    // draw quad
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    SDL_GL_SwapWindow(window);
  }

  sdlgl::cleanup();
  return EXIT_SUCCESS;
}
