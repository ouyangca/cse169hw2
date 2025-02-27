#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

class Particle {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force;
    float mass;
    bool isFixed;

    Particle(glm::vec3 pos, float m, bool fixed = false);

    void ApplyImpulse(const glm::vec3& impulse);
    void ApplyForce(const glm::vec3& f);  // Added ApplyForce method
    void HandleCollision();
    void Update(float deltaTime);

private:
    static constexpr float groundLevel = 0.0f;
    static constexpr float restitution = 0.5f;  // Coefficient of restitution (elasticity)
};

#endif // PARTICLE_H
