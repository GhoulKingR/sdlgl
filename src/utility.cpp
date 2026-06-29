#include <glad/glad.h>

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <glm/gtc/type_ptr.hpp>
#include <sdlgl/utility.hpp>
#include <string>

extern std::string _sdlglInternalErrorText;

uint32_t sdlgl::utility::shader::load(const char* vsource,
                                      const char* fsource) {
  assert(vsource != nullptr && fsource != nullptr &&
         "Missing vertex or fragment shader source codes");

  int success;
  char infoLog[512];

  const auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vsource, nullptr);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    _sdlglInternalErrorText = "Error :: VERTEX SHADER :: ";
    _sdlglInternalErrorText += infoLog;
    return 0;
  }

  const auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fsource, nullptr);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glDeleteShader(vertexShader);
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    _sdlglInternalErrorText = "Error :: FRAGMENT SHADER :: ";
    _sdlglInternalErrorText += infoLog;
    return 0;
  }

  auto shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    _sdlglInternalErrorText = "Error :: SHADER PROGRAM :: ";
    _sdlglInternalErrorText += infoLog;
    return 0;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  return shaderProgram;
}

void sdlgl::utility::shader::use(uint32_t program) { glUseProgram(program); }
void sdlgl::utility::shader::free(uint32_t program) {
  glDeleteProgram(program);
}
void sdlgl::utility::shader::setMat4(uint32_t program, const char* name,
                                     glm::mat4 data) {
  assert(name != nullptr);
  glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE,
                     glm::value_ptr(data));
}

void sdlgl::utility::shader::setInt(uint32_t program, const char* name,
                                    int data) {
  assert(name != nullptr);
  glUniform1i(glGetUniformLocation(program, name), data);
}

void sdlgl::utility::shader::setVec3(uint32_t program, const char* name,
                                     glm::vec3 data) {
  assert(name != nullptr);
  glUniform3f(glGetUniformLocation(program, name), data.x, data.y, data.z);
}

void sdlgl::utility::shader::setVec2(uint32_t program, const char* name,
                                     glm::vec2 data) {
  assert(name != nullptr);
  glUniform2f(glGetUniformLocation(program, name), data.x, data.y);
}

void sdlgl::utility::shader::setFloat(uint32_t program, const char* name,
                                      float data) {
  assert(name != nullptr);
  glUniform1f(glGetUniformLocation(program, name), data);
}
