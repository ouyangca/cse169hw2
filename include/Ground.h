#ifndef GROUND_H
#define GROUND_H


#include "core.h"
#include <glm/gtc/type_ptr.hpp> // For glm::value_ptr


class Ground {
public:
    Ground(float size = 100.0f); // Constructor with size parameter
    ~Ground(); // Destructor

    void Draw(const glm::mat4& viewProjMtx, GLuint shaderProgram); // Draw the ground

private:
    float size; // Size of the ground plane
    GLuint VAO, VBO; // OpenGL buffers
    glm::vec3 color; // Color of the ground

    void SetupMesh(); // Helper function to set up the ground mesh
};

#endif