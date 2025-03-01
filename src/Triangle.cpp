#include "Triangle.h"
#include <iostream>

Triangle::Triangle(bool skel_found, std::vector<glm::vec3> s_positions, 
    std::vector<glm::vec3> s_normals, std::vector<unsigned int> s_indices) {
    // Model matrix.
    model = glm::mat4(1.0f);

    // The color of the cube. Try setting it to something else!
    color = glm::vec3(1.0f, 0.95f, 0.1f);

    positions = s_positions;
    normals = s_normals;
    indices = s_indices;




    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);

    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Generate EBO, bind the EBO to the bound VAO and send the data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::cout << "triangle init success" << std::endl;
}

Triangle::~Triangle() {
    // Delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_positions);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Triangle::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    // actiavte the shader program
    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);
    std::cout << "triangle location sent to shader" << std::endl;


    // Bind the VAO
    glBindVertexArray(VAO);

    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    std::cout << "triangle gl element draw" << std::endl;


    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
    std::cout << "triangle draw success" << std::endl;

}


void Triangle::buildTriangle(bool skel_found, std::vector<Vertex*> *vertices, std::vector<unsigned int> *indicies) {
    // Model matrix.
    glm::mat4 modelMtx = glm::mat4(1.0f);

    // The color of the cube. Try setting it to something else!
    color = glm::vec3(0.6f, 0.6f, 1.0f);

    // Specify vertex positions

    // Specify normals

    // Specify indices
    

    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
    GLuint posLoc = 0;
    glEnableVertexAttribArray(posLoc);
    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    GLuint normLoc = 1;
    glEnableVertexAttribArray(normLoc);
    glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind the EBO to the bound VAO and send the data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}