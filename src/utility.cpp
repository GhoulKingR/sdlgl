#include <glad/glad.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <glm/gtc/type_ptr.hpp>
#include <sdlgl/utility.hpp>
#include <string>
#include <utility>
#include <vector>

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

struct BufferObject {
  bool active = false;
  uint32_t VBO = 0;
  uint32_t EBO = 0;
  uint32_t VAO = 0;
  size_t size = 0;

  BufferObject() = default;
  ~BufferObject() {
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteBuffers(1, &VAO);
    if (EBO) glDeleteBuffers(1, &EBO);
  }

  BufferObject(const BufferObject&) = delete;
  BufferObject operator=(const BufferObject&) = delete;
  BufferObject(BufferObject&& obj) {
    VBO = obj.VBO;
    VAO = obj.VAO;
    EBO = obj.EBO;
    size = obj.size;
    active = obj.active;
    obj.VBO = 0;
    obj.VAO = 0;
    obj.EBO = 0;
    obj.active = false;
  }

  BufferObject& operator=(BufferObject&& obj) {
    VBO = obj.VBO;
    VAO = obj.VAO;
    EBO = obj.EBO;
    size = obj.size;
    active = obj.active;
    obj.VBO = 0;
    obj.VAO = 0;
    obj.EBO = 0;
    obj.active = false;
    return *this;
  }

  void reset() {
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteBuffers(1, &VAO);
    if (EBO) glDeleteBuffers(1, &EBO);
    VBO = 0;
    VAO = 0;
    EBO = 0;
    active = false;
  }
};
static std::vector<BufferObject> objects;
static uint32_t openSpace = 0;

uint32_t sdlgl::utility::vertices::load(
    const std::vector<float>& vertices, const std::vector<uint32_t>& indices,
    const std::vector<attributes>& attribs) {
  BufferObject obj;
  glGenBuffers(1, &obj.VBO);
  glGenBuffers(1, &obj.EBO);
  glGenVertexArrays(1, &obj.VAO);
  glBindVertexArray(obj.VAO);

  glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
               vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]),
               indices.data(), GL_STATIC_DRAW);
  obj.size = indices.size();
  obj.active = true;

  for (auto& att : attribs) {
    glVertexAttribPointer(att.index, att.size, att.type, att.normalize,
                          att.stride, att.pointer);
    glEnableVertexAttribArray(att.index);
  }
  glBindVertexArray(0);

  if (openSpace > 0) {
    for (uint32_t i = 0; i < objects.size(); i++) {
      auto& o = objects[i];
      if (o.active) continue;

      o = std::move(obj);
      openSpace--;
      return i + 1;
    }
  } else {
    objects.push_back(std::move(obj));
    return objects.size();
  }

  return 0;
}

uint32_t sdlgl::utility::vertices::load(
    const std::vector<float>& vertices,
    const std::vector<attributes>& attribs) {
  BufferObject obj;
  glGenBuffers(1, &obj.VBO);
  glGenVertexArrays(1, &obj.VAO);
  glBindVertexArray(obj.VAO);

  glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
               vertices.data(), GL_STATIC_DRAW);
  obj.size = vertices.size();
  obj.active = true;

  for (auto& att : attribs) {
    glVertexAttribPointer(att.index, att.size, att.type, att.normalize,
                          att.stride, att.pointer);
    glEnableVertexAttribArray(att.index);
  }
  glBindVertexArray(0);

  if (openSpace > 0) {
    for (uint32_t i = 0; i < objects.size(); i++) {
      auto& o = objects[i];
      if (o.active) continue;

      o = std::move(obj);
      openSpace--;
      return i + 1;
    }
  } else {
    objects.push_back(std::move(obj));
    return objects.size();
  }

  return 0;
}

void sdlgl::utility::vertices::draw(uint32_t obj) {
  assert(obj > 0 && obj <= objects.size() && "Invalid object");
  auto& o = objects[obj - 1];
  assert(o.active && "Invalid object");

  glBindVertexArray(o.VAO);
  if (o.EBO) {
    glDrawElements(GL_TRIANGLES, o.size, GL_UNSIGNED_INT, 0);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, o.size);
  }
}

void sdlgl::utility::vertices::free(uint32_t obj) {
  assert(obj > 0 && obj <= objects.size() && "Invalid object");
  objects[obj - 1].reset();
}
