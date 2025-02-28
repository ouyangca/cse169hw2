#include "SpringDamper.h"

SpringDamper::SpringDamper(Particle* a, Particle* b, float k, float d)
    : p1(a), p2(b), ks(k), kd(d) {
    restLength = glm::length(p1->position - p2->position);
}

// void SpringDamper::ComputeForce() {
//     glm::vec3 dir = p2->position - p1->position;
//     float length = glm::length(dir);
//     if (length == 0) return;

//     glm::vec3 e = dir / length;
    


    
    
//     dir = glm::normalize(dir);
//     float displacement = length - restLength;


//     // Damping force
//     float relativeVelocity = glm::dot((p2->velocity - p1->velocity), e);

//     float f = -stiffness * (glm::distance(p1->position, p2->position) - length);
//     float damper = -dampingFactor * relativeVelocity;


//     p1->ApplyForce(f + damper);
//     p2->ApplyForce(-f - damper);
// }



void SpringDamper::ComputeForce() {
    // Step 1: Compute current length and unit direction vector
    glm::vec3 e_star = p2->position - p1->position; // e* = r2 - r1
    float length = glm::length(e_star);             // l = |e*|

    if (length == 0.0f) return; // Avoid division by zero

    glm::vec3 e = e_star / length; // Unit vector e = e* / l

    // Step 2: Compute closing velocity
    float v_close = glm::dot((p1->velocity - p2->velocity), e); // v_close = (v1 - v2) · e

    // Step 3: Compute final force magnitude

    float force_magnitude = (-ks * (restLength - length)) - (kd * v_close);

    // Compute force vectors
    glm::vec3 f1 = force_magnitude * e;  // f1 = f * e
    glm::vec3 f2 = -f1;                  // f2 = -f1

    // Apply forces to the particles
    p1->ApplyForce(f1);
    p2->ApplyForce(f2);
}