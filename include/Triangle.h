#pragma once
#include <glm/glm.hpp>
#include "Vertex.h"

class Triangle {
public:
    int v0, v1, v2; // Indices of vertices forming the triangle
    glm::vec3 normal; // Face normal

    Triangle(int v0, int v1, int v2);
    void computeNormal(Vertex* vertices);
};
