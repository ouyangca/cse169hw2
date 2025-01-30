#include "Vertex.h"

Vertex::Vertex() {
    
}

void Vertex::setPositions(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}

void Vertex::setNormals(float nx, float ny, float nz) {
    normal = glm::vec3(nx, ny, nz);
}

void Vertex::setShaderPositions(float x, float y, float z) {
    shader_position = glm::vec3(x, y, z);
}

void Vertex::setShaderNormals(float nx, float ny, float nz) {
    shader_normal = glm::vec3(nx, ny, nz);
}