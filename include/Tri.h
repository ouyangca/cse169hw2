#ifndef TRI_H
#define TRI_H

#include "Particle.h"

class Tri {
public:
    Particle* p1;
    Particle* p2;
    Particle* p3;

    Tri(Particle* a, Particle* b, Particle* c);
    
    glm::vec3 ComputeNormal() const;
    void ApplyAerodynamicForce(const glm::vec3& windVelocity, float dragCoefficient);
};

#endif
