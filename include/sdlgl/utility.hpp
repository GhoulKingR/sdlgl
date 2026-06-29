#pragma once

#include <cstdint>
#include <glm/glm.hpp>

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
}  // namespace utility
}  // namespace sdlgl
