#include "Cloth.h"
#include <iostream>


Cloth::Cloth(int w, int h, float mass, float stiffness, float damping, float wind)
    : width(w), height(h) {
    Initialize();
    SetupMesh();
}

void Cloth::Initialize() {
    particles.clear();
    springs.clear();
    triangles.clear();
    s_positions.clear();
    s_normals.clear();
    s_indices.clear();

    // Create particles
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            glm::vec3 position = glm::vec3(x * 0.1f, height * 0.1f - y * 0.1f, 0.0f);
            bool fixed = (y == 0);  // Fix the top row
            particles.emplace_back(position, 1.0f, fixed);
        }
    }

    // Create springs (structural, shear, and bend)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x < width - 1) // Horizontal
                springs.emplace_back(&particles[GetIndex(x, y)], &particles[GetIndex(x + 1, y)], 500.0f, 0.1f);
            if (y < height - 1) // Vertical
                springs.emplace_back(&particles[GetIndex(x, y)], &particles[GetIndex(x, y + 1)], 500.0f, 0.1f);
            if (x < width - 1 && y < height - 1) { // Diagonal (shear)
                springs.emplace_back(&particles[GetIndex(x, y)], &particles[GetIndex(x + 1, y + 1)], 500.0f, 0.1f);
                springs.emplace_back(&particles[GetIndex(x + 1, y)], &particles[GetIndex(x, y + 1)], 500.0f, 0.1f);
            }
        }
    }
    // std::cout << "Init." << std::endl;


    // Create triangle mesh for rendering & physics
    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {
            unsigned int i1 = GetIndex(x, y);
            unsigned int i2 = GetIndex(x + 1, y);
            unsigned int i3 = GetIndex(x, y + 1);
            unsigned int i4 = GetIndex(x + 1, y + 1);

            s_indices.push_back(i1);
            s_indices.push_back(i2);
            s_indices.push_back(i3);

            s_indices.push_back(i3);
            s_indices.push_back(i2);
            s_indices.push_back(i4);

            triangles.emplace_back(&particles[i1], &particles[i2], &particles[i3]);
            triangles.emplace_back(&particles[i3], &particles[i2], &particles[i4]);
        }
    }

    std::cout << "Init successful." << std::endl;

}

void Cloth::ApplyForces(const glm::vec3& gravity, const glm::vec3& windDirection, float windSpeed) {
    for (auto& p : particles) {
        if (!p.isFixed) {
            // Apply gravity
            p.ApplyForce(gravity * p.mass);

            // Apply wind force
            glm::vec3 windForce = windDirection * windSpeed;
            p.ApplyForce(windForce);
        }
    }

    // Apply aerodynamic forces using Tri
    for (auto& t : triangles) {
        t.ApplyAerodynamicForce(windDirection * windSpeed, 0.2f);
    }
}

void Cloth::SetupMesh() {
    // std::vector<float> vertices;
    // indices.clear();

    // // Fill vertices with particle positions
    // for (const auto& particle : particles) {
    //     vertices.push_back(particle.position.x);
    //     vertices.push_back(particle.position.y);
    //     vertices.push_back(particle.position.z);
    // }

    // // Fill indices for triangles
    // for (const auto& tri : triangles) {
    //     indices.push_back(GetIndex(tri.p1->position.x, tri.p1->position.y));
    //     indices.push_back(GetIndex(tri.p2->position.x, tri.p2->position.y));
    //     indices.push_back(GetIndex(tri.p3->position.x, tri.p3->position.y));
    // }
}

void Cloth::Update(float deltaTime) {
    for (auto& spring : springs) {
        spring.ComputeForce();
    }

    for (auto& p : particles) {
        p.Update(deltaTime);
    }

    UpdateMesh();
}

void Cloth::UpdateMesh() {
    s_positions.clear();
    s_normals.clear();

    // Loop through particles and store positions
    for (const auto& p : particles) {
        s_positions.push_back(p.position);
    }

    // Compute smooth normals by averaging triangle normals
    std::vector<glm::vec3> temp_normals(particles.size(), glm::vec3(0.0f));

    for (const auto& tri : triangles) {
        glm::vec3 normal = tri.ComputeNormal();
        temp_normals[GetIndex(tri.p1->position.x, tri.p1->position.y)] += normal;
        temp_normals[GetIndex(tri.p2->position.x, tri.p2->position.y)] += normal;
        temp_normals[GetIndex(tri.p3->position.x, tri.p3->position.y)] += normal;
    }

    for (auto& n : temp_normals) {
        s_normals.push_back(glm::normalize(n));
    }
}

// OpenGL Rendering
void Cloth::Draw(const glm::mat4& viewProjMtx, GLuint shaderProgram) {
    // Update render data
    // std::vector<glm::vec3> positions;
    // std::vector<glm::vec3> normals;
    // positions.reserve(particles.size());
    // normals.reserve(particles.size());

    // for (const auto& p : particles) {
    //     positions.push_back(p.position);
    //     normals.push_back(p.normal);
    // }

    // Update VBO
    // glm::vec3 color = glm::vec3(1, 0, 0);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * s_positions.size(), s_positions.data());
    // glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * s_positions.size(), 
    //                 sizeof(glm::vec3) * s_normals.size(), s_normals.data());

    // // Set shader uniforms
    // glUseProgram(shaderProgram);
    // glm::mat4 model(1.0f);
    // glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewProj"), 1, GL_FALSE, glm::value_ptr(viewProjMtx));
    // glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    // glUniform3fv(glGetUniformLocation(shaderProgram, "DiffuseColor"), 1, glm::value_ptr(color));

    // // Draw
    // glBindVertexArray(VAO);
    // glDrawElements(GL_TRIANGLES, s_indices.size(), GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0);

    
    
    // glUseProgram(0);

    // std::cout << "draw" << std::endl;


    // renderTriangles = new Triangle(true, s_positions, s_normals, s_indices);   
    // renderTriangles->draw(viewProjMtx, shaderProgram);

    // delete renderTriangles;
    // renderTriangles = nullptr;

    
    std::cout << "draw" << std::endl;

}

int Cloth::GetIndex(int x, int y) const {
    return y * width + x;
}


void Cloth::MoveFixedPoint(int index, glm::vec3 delta) {
    if (index >= 0 && index < particles.size() && particles[index].isFixed) {
        particles[index].position += delta;
    }
}

int Cloth::GetNumFixedPoints() const {
    int count = 0;
    for (const auto& p : particles) {
        if (p.isFixed) count++;
    }
    return count;
}
