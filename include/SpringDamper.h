#ifndef SPRING_DAMPER_H
#define SPRING_DAMPER_H

#include "Particle.h"

class SpringDamper {
public:
    Particle* p1;
    Particle* p2;
    float restLength;
    float stiffness;
    float dampingFactor;

    SpringDamper(Particle* a, Particle* b, float k, float d);

    void ComputeForce();
};

#endif
