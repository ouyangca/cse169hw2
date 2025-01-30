#pragma once

#include <vector>

#include "core.h"
#include "Joint.h"
#include <string>  // Required for std::string
#include <iostream> // For std::cout, std::endl


class Skeleton {
public:
    Skeleton();
    ~Skeleton();
    Joint* root;
    std::vector<Joint*> joints; // Flat list of all joints

    // ○ Reading in .skel files through load()
    bool load(const char* skelFile);
    // ○ Building skeleton tree structure (bones + joints)
    void update();
    // ○ Computing local/world matrices through update()
    // ○ Rendering everything through draw()

    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    glm::mat4 GetWorldMatrix(int jointIndex); // Lookup by index
    void BuildJointVector(); // Populate the joint list
    void CollectJoints(Joint* node); // Helper for traversal
    
};

