#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Joint.h"

class Vertex {
public:
    glm::vec3 position; // Original position
    glm::vec3 shader_position; // Shader position

    glm::vec3 normal;   // Original normal
    glm::vec3 shader_normal;   // Shader normal

    std::vector<float> weights;  // Skinning weights per joint
    std::vector<Joint*> joints;  // Joints influencing this vertex

    Vertex();
    void setPositions(float x, float y, float z);
    void setShaderPositions(float x, float y, float z);
    void setNormals(float nx, float ny, float nz);
    void setShaderNormals(float nx, float ny, float nz);

};
