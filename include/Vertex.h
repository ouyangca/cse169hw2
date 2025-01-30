#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Joint.h"

class Vertex {
public:
    glm::vec3 position; // Original position
    glm::vec3 normal;   // Original normal

    std::vector<float> weights;  // Skinning weights per joint
    std::vector<Joint*> joints;  // Joints influencing this vertex

    Vertex();
    void setPositions(float x, float y, float z);
    void setNormals(float nx, float ny, float nz);
};
