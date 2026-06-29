#pragma once

#include <OpenGL/gltypes.h>

#include <cstdint>
#include <glm/glm.hpp>
#include <vector>

#include "sdlgl/sdlgl.hpp"

namespace sdlgl {
namespace utility {
namespace shader {
uint32_t load(const char* vShaderCode, const char* fShaderCode);
void free(uint32_t);
void use(uint32_t);
void setMat4(uint32_t program, const char* name, glm::mat4 data);
void setVec3(uint32_t program, const char* name, glm::vec3 data);
void setVec2(uint32_t program, const char* name, glm::vec2 data);
void setInt(uint32_t program, const char* name, int data);
void setFloat(uint32_t program, const char* name, float data);
}  // namespace shader

namespace vertices {
struct attributes {
  GLuint index;
  GLint size;
  GLenum type;
  GLboolean normalize;
  GLsizei stride;
  const void* pointer;
};
uint32_t load(const std::vector<float>& vertices,
              const std::vector<uint32_t>& indices,
              const std::vector<attributes>&);
uint32_t load(const std::vector<float>& vertices,
              const std::vector<attributes>&);
void draw(uint32_t);
void free(uint32_t);
}  // namespace vertices
}  // namespace utility
}  // namespace sdlgl
