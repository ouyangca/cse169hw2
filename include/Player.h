#pragma once
#include "Animation.h"
#include "Rig.h"
#include <GLFW/glfw3.h> // Include GLFW for time functions

class Player {
public:
    Player(Animation* clip, Rig* rig);
    ~Player();

    void Update(); // Update the animation
    void SetSpeed(float speedFactor); // Set the animation speed

private:
    Animation* clip; // Animation clip
    Rig* rig; // Rig (skeleton)
    float time_start; // Start time of the animation
    float time_end; // End time of the animation
    float currentTime; // Current time in the animation
    float playSpeed; // Speed factor for the animation
    double lastUpdateTime; // Last update time (for delta time calculation)
    std::vector<float> poses; // Poses for the skeleton
    glm::mat4 rootTranslation; // Root translation matrix
};