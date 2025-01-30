#include "Triangle.h"

Triangle::Triangle(int v0, int v1, int v2) {
    this->v0 = v0;
    this->v1 = v1;
    this->v2 = v2;
}

void Triangle::computeNormal(Vertex* vertices) {
    glm::vec3 edge1 = vertices[v1].position - vertices[v0].position;
    glm::vec3 edge2 = vertices[v2].position - vertices[v0].position;
    normal = glm::normalize(glm::cross(edge1, edge2));
}
