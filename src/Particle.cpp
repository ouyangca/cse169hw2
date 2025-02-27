#include "Particle.h"
#include <glm/glm.hpp>

Particle::Particle(glm::vec3 pos, float m, bool fixed)
    : position(pos), velocity(glm::vec3(0.0f)), force(glm::vec3(0.0f)), mass(m), isFixed(fixed) {}

void Particle::ApplyImpulse(const glm::vec3& impulse) {
    if (!isFixed) {
        velocity += impulse / mass;
    }
}

void Particle::ApplyForce(const glm::vec3& f) {
    if (!isFixed) {
        force += f;
    }
}

void Particle::HandleCollision() {
    if (position.y < groundLevel) {
        // Move particle above ground
        position.y = groundLevel;

        // Reflect velocity with some energy loss
        velocity.y = -velocity.y * restitution;
    }
}

void Particle::Update(float deltaTime) {
    if (!isFixed) {
        // Apply force-based acceleration
        glm::vec3 acceleration = force / mass;
        velocity += acceleration * deltaTime;

        // Apply gravity
        velocity += glm::vec3(0.0f, -9.81f, 0.0f) * deltaTime;
        position += velocity * deltaTime;

        // Reset forces after each update step
        force = glm::vec3(0.0f);

        // Check for collision with the ground
        HandleCollision();
    }
}
