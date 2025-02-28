#include "Cloth.h"
#include <glm/gtc/type_ptr.hpp> // For glm::value_ptr
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
            bool fixed = (y == 0);

            particles.emplace_back(position, 1.0f, fixed);
        }
    }

    // Create springs (structural, shear, and bend)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x < width - 1) // Horizontal
                springs.emplace_back(&particles[GetIndex(x, y)], &particles[GetIndex(x + 1, y)], 2500.0f, 24.0f);
            if (y < height - 1) // Vertical
                springs.emplace_back(&particles[GetIndex(x, y)], &particles[GetIndex(x, y + 1)], 2500.0f, 24.0f);
            if (x < width - 1 && y < height - 1) { // Diagonal (shear)
                springs.emplace_back(&particles[GetIndex(x, y)], &particles[GetIndex(x + 1, y + 1)], 5.0f, 0.5f);
                springs.emplace_back(&particles[GetIndex(x + 1, y)], &particles[GetIndex(x, y + 1)], 5.0f, 0.5f);
            }
        }
    }


    // Create triangle mesh for rendering & physics
    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {
            unsigned int i1 = GetIndex(x, y);
            unsigned int i2 = GetIndex(x + 1, y);
            unsigned int i3 = GetIndex(x, y + 1);
            unsigned int i4 = GetIndex(x + 1, y + 1);

            if (i1 >= particles.size() || i2 >= particles.size() || i3 >= particles.size() || i4 >= particles.size()) {
            std::cout << "Error: Invalid triangle index detected." << std::endl;
        }

            s_indices.push_back(i1);
            s_indices.push_back(i2);
            s_indices.push_back(i3);

            s_indices.push_back(i3);
            s_indices.push_back(i2);
            s_indices.push_back(i4);

            triangles.emplace_back(&particles[i1], &particles[i2], &particles[i3], i1, i2, i3);
            triangles.emplace_back(&particles[i3], &particles[i2], &particles[i4], i3, i2, i4);
        }
    }

    std::cout << "Init successful." << std::endl;

}

void Cloth::ApplyForces(const glm::vec3& gravity, const glm::vec3& windDirection, float windSpeed) {

    
}

void Cloth::SetupMesh() {
    
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
        std::cout << "Triangle normal: (" << normal.x << ", " << normal.y << ", " << normal.z << ")" << std::endl;
        // Use precomputed indices for the particles
        temp_normals[tri.p1_index] += normal;
        temp_normals[tri.p2_index] += normal;
        temp_normals[tri.p3_index] += normal;
    }

    for (auto& n : temp_normals) {
    if (glm::length(n) > 1e-6f) {
        s_normals.push_back(glm::normalize(n));
    } else {
        s_normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); // Default normal
    }
}



    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, s_positions.size() * sizeof(glm::vec3), s_positions.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_indices.size() * sizeof(unsigned int), s_indices.data(), GL_STATIC_DRAW);

    // Position Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


}

void Cloth::Update(float deltaTime) {

    

    for (auto& p : particles) {
        p.force = glm::vec3(0);

    }

    for (auto& spring : springs) {
        spring.ComputeForce();
    }


    glm::vec3 force(0.0f, -0.0f, 0.0f);  // Gravity pulls downward
    glm::vec3 windDirection(1.0f, 1.0f, 0.0f); // Wind blowing in the +X direction
    float windSpeed = 10/60.0f; // Adjustable wind speed

    // Call ApplyForces to apply physics
    for (auto& t : triangles) {
        t.ApplyAerodynamicForce(windDirection * windSpeed, 1.0f);
    }

    for (auto& p : particles) {
        p.Update(deltaTime);
    }

    for (auto& p : particles) {
        std::cout << "Particle force: (" << p.force.x << ", " << p.force.y << ", " << p.force.z << ")" << std::endl;
        std::cout << "Particle position: (" << p.position.x << ", " << p.position.y << ", " << p.position.z << ")" << std::endl;
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
        // std::cout << normal[]

        // Use precomputed indices for the particles
        temp_normals[tri.p1_index] += normal;
        temp_normals[tri.p2_index] += normal;
        temp_normals[tri.p3_index] += normal;

        
    }
    for (auto& n : temp_normals) {
        if (glm::length(n) > 1e-6f) {
            s_normals.push_back(glm::normalize(n));
        } else {
            s_normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); // Default normal
        }
    }

}

// void Cloth::Draw(const glm::mat4& viewProjMtx, GLuint shaderProgram) {
//     // Ensure mesh is properly updated
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * s_positions.size(), s_positions.data());
//     glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * s_positions.size(), 
//                     sizeof(glm::vec3) * s_normals.size(), s_normals.data());

//     // Set shader uniforms
//     glUseProgram(shaderProgram);
//     glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewProj"), 1, GL_FALSE, glm::value_ptr(viewProjMtx));
    
//     glm::mat4 model(1.0f);
//     glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

//     glm::vec3 color = glm::vec3(1.0f, 0.95f, 0.1f);
//     glUniform3fv(glGetUniformLocation(shaderProgram, "DiffuseColor"), 1, glm::value_ptr(color));

//     // Draw cloth
//     glBindVertexArray(VAO);
//     glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(s_indices.size()), GL_UNSIGNED_INT, 0);
//     glBindVertexArray(0);
    
//     glUseProgram(0);

//     // Debugging Info
//     std::cout << "Positions size: " << s_positions.size() << std::endl;
    
//     std::cout << "Normals size: " << s_normals.size() << std::endl;
//     std::cout << "Indices size: " << s_indices.size() << std::endl;
// }


// // OpenGL Rendering
void Cloth::Draw(const glm::mat4& viewProjMtx, GLuint shaderProgram) {
//     // Update render data
    // std::vector<glm::vec3> positions;
    // std::vector<glm::vec3> normals;
    // positions.reserve(particles.size());
    // normals.reserve(particles.size());

    // for (const auto& p : particles) {
    //     positions.push_back(p.position);
    //     normals.push_back(p.normal);
    // }
    // model = glm::mat4(1.0f);

    glm::mat4 model(1.0f);
    glm::vec3 color = glm::vec3(1.0f, 0.95f, 0.1f);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * s_positions.size(), s_positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * s_positions.size(), 
                    sizeof(glm::vec3) * s_normals.size(), s_normals.data());

    // Set shader uniforms
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewProj"), 1, GL_FALSE, glm::value_ptr(viewProjMtx));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(shaderProgram, "DiffuseColor"), 1, glm::value_ptr(color));

    // Draw
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, s_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    

    // std::cout << "Positions size: " << s_positions.size() << std::endl;
    // std::cout << "Normals size: " << s_normals.size() << std::endl;
    // std::cout << "Indices size: " << s_indices.size() << std::endl;
    // std::cout << "Positions: " << s_positions[200][0] << " " << s_positions[200][1] << " " << s_positions[200][2]<< std::endl;
    // std::cout << "Normal: " << s_normals[200][0] << " " << s_normals[200][1] << " " << s_normals[200][2]<< std::endl;

    // std::cout << "Indices: " << s_indices[0] << " " << s_indices[1] << " " << s_indices[2]<< std::endl;


}

int Cloth::GetIndex(int x, int y) const {
    return y * width + x;
}


void Cloth::MoveFixedPoint(int index, glm::vec3 delta) {
    for (auto& p : particles) {
        if(p.isFixed) p.position += delta;
    }
}

int Cloth::GetNumFixedPoints() const {
    int count = 0;
    for (const auto& p : particles) {
        if (p.isFixed) count++;
    }
    return count;
}
