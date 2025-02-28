#ifndef CLOTH_H
#define CLOTH_H

// Include necessary libraries
#include <vector>
#include <glm/glm.hpp>
#include "Particle.h"
#include "Triangle.h"

#include "SpringDamper.h"
#include "Tri.h"

class Cloth {
public:
    int width, height;  // Grid dimensions
    float windSpeed;
    std::vector<Particle> particles;
    std::vector<SpringDamper> springs;
    std::vector<Tri> triangles;
	Triangle* renderTriangles;



    // Triangle information for rendering
    std::vector<glm::vec3> s_positions;
    std::vector<glm::vec3> s_normals;
    std::vector<unsigned int> s_indices;

    // Constructor
    Cloth(int w, int h, float mass, float stiffness, float damping, float wind);

    // Initialization
    void Initialize();

    // Apply forces (gravity + wind combined)
    void ApplyForces(const glm::vec3& gravity, const glm::vec3& windDirection, float windSpeed);

    // Update physics
    void Update(float deltaTime);

    // Handle ground collisions
    void HandleCollisions(float groundHeight);

    // Move fixed particles based on user input
    void MoveFixedPoint(int index, glm::vec3 delta);

    // Get number of fixed points
    int GetNumFixedPoints() const;

    // Render the cloth using OpenGL
    void Draw(const glm::mat4& viewProjMtx, GLuint shaderProgram);

    void SetWindSpeed(float speed);


    void ReleaseAllParticles();  // Function to make all particles non-fixed



private:
    // Helper function to convert 2D grid indices into 1D array index
    int GetIndex(int x, int y) const;

    // OpenGL Buffers
    GLuint VAO, VBO, EBO;
    
    // Initialize OpenGL buffers
    void SetupMesh();

    // Update OpenGL vertex buffer and store Tri data
    void UpdateMesh();
};

#endif // CLOTH_H
