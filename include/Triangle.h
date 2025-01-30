#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>
#include "Vertex.h"
#include "core.h"



class Triangle {
public:
    // Constructor and Destructor
    Triangle(bool skel_found, std::vector<Vertex*> vertices, std::vector<unsigned int> indices);
    ~Triangle();

    // Draw the triangle
    void draw(const glm::mat4& viewProjMtx, GLuint shader);

    // Build the triangle (used for initialization)
    void buildTriangle(bool skel_found, std::vector<Vertex*> *vertices, std::vector<unsigned int> *indices);

private:
    // Model matrix for transformations
    glm::mat4 model;

    // Color of the triangle
    glm::vec3 color;

    // Vertex positions, normals, and indices
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    // OpenGL buffers and objects
    GLuint VAO; // Vertex Array Object
    GLuint VBO_positions; // Vertex Buffer Object for positions
    GLuint VBO_normals; // Vertex Buffer Object for normals
    GLuint EBO; // Element Buffer Object for indices
};

#endif // TRIANGLE_H