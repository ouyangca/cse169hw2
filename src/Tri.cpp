#include "Tri.h"

Tri::Tri(Particle* a, Particle* b, Particle* c)
    : p1(a), p2(b), p3(c) {}

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
