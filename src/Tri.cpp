#include "Tri.h"

Tri::Tri(Particle* p1, Particle* p2, Particle* p3, int p1_index, int p2_index, int p3_index)
        : p1(p1), p2(p2), p3(p3), p1_index(p1_index), p2_index(p2_index), p3_index(p3_index) {}

glm::vec3 Tri::ComputeNormal() const {
    glm::vec3 v1 = p2->position - p1->position;
    glm::vec3 v2 = p3->position - p1->position;
    return glm::normalize(glm::cross(v1, v2));
}

void Tri::ApplyAerodynamicForce(const glm::vec3& windVelocity, float dragCoefficient) {
    glm::vec3 normal = ComputeNormal();
    glm::vec3 velocity = (p1->velocity + p2->velocity + p3->velocity) / 3.0f;  // Average velocity
    glm::vec3 relativeWind = windVelocity - velocity;
    float pressure = glm::dot(relativeWind, normal) * dragCoefficient;

    glm::vec3 force = normal * pressure;
    p1->ApplyForce(force / 3.0f);
    p2->ApplyForce(force / 3.0f);
    p3->ApplyForce(force / 3.0f);
}
