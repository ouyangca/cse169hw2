#include "Player.h"
#include <iostream>

// Constructor
Player::Player(Animation* clip, Rig* rig) {
    this->clip = clip;
    this->rig = rig;
    time_start = clip->time_start;
    time_end = clip->time_end;
    currentTime = time_start - 5;
    playSpeed = 1.0f; // Default speed (normal speed)
    lastUpdateTime = glfwGetTime(); // Initialize last update time

    // Initialize poses to 0s so that index can be used later
    for (int i = 0; i < (3 * rig->skeleton->joints.size() + 3); i++)
        poses.push_back(0.0f);
}

// Destructor
Player::~Player() {
    // Clean up if necessary
}

// Set the animation speed
void Player::SetSpeed(float speedFactor) {
    playSpeed = speedFactor;
}

// Update the animation
void Player::Update() {
    // Calculate delta time
    double currentTimeGLFW = glfwGetTime();
    double deltaTime = currentTimeGLFW - lastUpdateTime;
    lastUpdateTime = currentTimeGLFW;

    // Update currentTime based on delta time and play speed
    currentTime += static_cast<float>(deltaTime) * playSpeed;

    // // Loop the animation if it reaches the end
    // if (currentTime > time_end) {
    //     currentTime = time_start;
    // }

    // Evaluate the animation at the current time
    clip->Evaluate(currentTime, poses);

    // Update root translation matrix
    rootTranslation = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        poses[0], poses[1], poses[2], 1.0f
    };

    // Update joint DOFs
    for (int i = 0; i < rig->skeleton->joints.size(); i++) {
        rig->skeleton->joints[i]->JointDOF[0]->SetValue(poses[3 + i * 3]);
        rig->skeleton->joints[i]->JointDOF[1]->SetValue(poses[3 + i * 3 + 1]);
        rig->skeleton->joints[i]->JointDOF[2]->SetValue(poses[3 + i * 3 + 2]);
    }
}