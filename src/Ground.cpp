#include "Ground.h"
#include <iostream>

Ground::Ground(float size) : size(size), color(0.0f, 0.0f, 1.0f) {
    SetupMesh();
}

Ground::~Ground() {
    // Clean up OpenGL buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Ground::SetupMesh() {
    // Define the vertices of the ground plane (a large quad)
    float halfSize = size / 2.0f;
    float vertices[] = {
        // Positions          // Normals (pointing up)
        -halfSize, 0.0f, -halfSize,  0.0f, 1.0f, 0.0f,
         halfSize, 0.0f, -halfSize,  0.0f, 1.0f, 0.0f,
         halfSize, 0.0f,  halfSize,  0.0f, 1.0f, 0.0f,
        -halfSize, 0.0f,  halfSize,  0.0f, 1.0f, 0.0f
    };

    // Define the indices for the quad (two triangles)
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Generate and bind the VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind the VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Generate and bind the EBO
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set up vertex attribute pointers
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind the VAO
    glBindVertexArray(0);
}

void Ground::Draw(const glm::mat4& viewProjMtx, GLuint shaderProgram) {
    // Bind the shader program
    glUseProgram(shaderProgram);

    // Set the model matrix (identity matrix for the ground)
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // Set the view-projection matrix
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewProj"), 1, GL_FALSE, glm::value_ptr(viewProjMtx));

    // Set the ground color
    glUniform3fv(glGetUniformLocation(shaderProgram, "DiffuseColor"), 1, glm::value_ptr(color));

    // Bind the VAO
    glBindVertexArray(VAO);

    // Draw the ground (two triangles)
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the shader program
    glUseProgram(0);
}