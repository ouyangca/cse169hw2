#include "Tri.h"
#include <iostream>

Tri::Tri(Particle* p1, Particle* p2, Particle* p3, int p1_index, int p2_index, int p3_index)
        : p1(p1), p2(p2), p3(p3), p1_index(p1_index), p2_index(p2_index), p3_index(p3_index) {}



glm::vec3 Tri::ComputeNormal() const {
    glm::vec3 v1 = p2->position - p1->position;
    glm::vec3 v2 = p3->position - p1->position;
    glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

    return normal;

    // Debug: Print edge vectors and normal
    // std::cout << "Edge1: (" << edge1.x << ", " << edge1.y << ", " << edge1.z << ")" << std::endl;
    // std::cout << "Edge2: (" << edge2.x << ", " << edge2.y << ", " << edge2.z << ")" << std::endl;
    // std::cout << "Normal: (" << normal.x << ", " << normal.y << ", " << normal.z << ")" << std::endl;

}

void Tri::ApplyAerodynamicForce(const glm::vec3& windVelocity, float dragCoefficient) {
    // const float airDensity = 1.225f;
    glm::vec3 normal = ComputeNormal();
    // glm::vec3 v_surface = (p1->velocity + p2->velocity + p3->velocity) / 3.0f;
    // glm::vec3 v_relative = windVelocity - v_surface;

    // float v_relative_mag = glm::length(v_relative);
    // glm::vec3 v_relative_dir = v_relative / v_relative_mag; 

    // if (v_relative_mag == 0.0f) return;

    // // compute trangle area & cross-sectional area
    // glm::vec3 edge1 = p2->position - p1->position;
    // glm::vec3 edge2 = p3->position - p1->position;
    // float baseArea = 0.5f * glm::length(glm::cross(edge1, edge2));

    // float projectedArea = baseArea * glm::dot(v_relative_dir, normal);
    // projectedArea = glm::max(projectedArea, 0.0f);
    // float forceMag = -0.5f * airDensity * v_relative_mag * v_relative_mag * dragCoefficient * projectedArea;
    // glm::vec3 force = forceMag * normal;

    // std::cout << "Aerodynamic force: (" << force.x << ", " << force.y << ", " << force.z << ")" << std::endl;
    // std::cout << "Aerodynamic force: (" << forceMag <<  ")" << std::endl;
    // std::cout << "relative force: (" << v_relative_mag <<  ")" << std::endl;
    // std::cout << "projected area: (" << projectedArea  <<  ")" << std::endl;
    // std::cout << "base area: (" << baseArea  <<  ")" << std::endl;


    // std::cout << "wind force: (" << windVelocity[0]<< windVelocity[1]<< windVelocity[2] <<  ")" << std::endl;
    // std::cout << "relative force: (" << v_relative[0]<< v_relative[1]<< v_relative[2] <<  ")" << std::endl;
    




    // p1->ApplyForce(force / 3.0f);
    // p2->ApplyForce(force / 3.0f);
    // p3->ApplyForce(force / 3.0f);





    glm::vec3 v_surface = (p1->velocity + p2->velocity + p3->velocity) / 3.0f;
    
    glm::vec3 v = v_surface - windVelocity;
    float v_mag = glm::length(v);
    if (v_mag < 1e-6f) return;  

    glm::vec3 edge1 = p2->position - p1->position;
    glm::vec3 edge2 = p3->position - p1->position;
    float a0 = 0.5f * glm::length(glm::cross(edge1, edge2));

    glm::vec3 v_dir = v / v_mag;
    float a = a0 * glm::dot(v_dir, normal);
    
    const float rho = 1.225f; 
    const float cd = 1.0f;    
    glm::vec3 force = 0.5f * rho * (v_mag*v_mag) * cd * a * normal;
    
    p1->force += force / 3.0f;
    p2->force += force / 3.0f;
    p3->force += force / 3.0f;
}

