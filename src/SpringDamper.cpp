#include "SpringDamper.h"

SpringDamper::SpringDamper(Particle* a, Particle* b, float k, float d)
    : p1(a), p2(b), stiffness(k), dampingFactor(d) {
    restLength = glm::length(p1->position - p2->position);
}

void SpringDamper::ComputeForce() {
    glm::vec3 dir = p2->position - p1->position;
    float length = glm::length(dir);
    if (length == 0) return;
    
    dir = glm::normalize(dir);
    float displacement = length - restLength;

    // Spring force
    glm::vec3 springForce = -stiffness * displacement * dir;

    // Damping force
    glm::vec3 relativeVelocity = p2->velocity - p1->velocity;
    float damping = glm::dot(relativeVelocity, dir) * dampingFactor;
    glm::vec3 dampingForce = -damping * dir;

    p1->ApplyForce(springForce + dampingForce);
    p2->ApplyForce(-springForce - dampingForce);
}
