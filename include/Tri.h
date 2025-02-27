#ifndef TRI_H
#define TRI_H

#include "Particle.h"

class Tri {
public:
    Particle* p1;
    Particle* p2;
    Particle* p3;
    int p1_index; // Index of p1 in the particles array
    int p2_index; // Index of p2 in the particles array
    int p3_index; // Index 

    Tri(Particle* p1, Particle* p2, Particle* p3, int p1_index, int p2_index, int p3_index);
    
    glm::vec3 ComputeNormal() const;
    void ApplyAerodynamicForce(const glm::vec3& windVelocity, float dragCoefficient);
};

#endif
