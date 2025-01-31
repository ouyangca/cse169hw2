#include "Vertex.h"

Vertex::Vertex() {
    position = glm::vec3(0.0f);
    normal = glm::vec3(0.0f);
}

void Vertex::setPositions(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}

void Vertex::setNormals(float nx, float ny, float nz) {
    normal = glm::vec3(nx, ny, nz);
}

