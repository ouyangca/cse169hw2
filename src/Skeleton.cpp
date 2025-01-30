#pragma once

#include <vector>
#include <string>  // Required for std::string
#include <iostream> // For std::cout, std::endl
#include "Skeleton.h"

#include "core.h"

Skeleton::Skeleton() {root = NULL;  }
Skeleton::~Skeleton() {}

// ○ Reading in .skel files through load()
// ○ Building skeleton tree structure (bones + joints)
bool Skeleton::load(const char* skelFile){
    Tokenizer token;
    token.Open(skelFile);
    token.FindToken("balljoint");

    // Parse tree
    root = new Joint();
    root->Load(token);

    // Build a flat vector of joints for quick lookup
    BuildJointVector();

    // Finish
    token.Close();
    return true;
}


        
// ○ Computing local/world matrices through update()
void Skeleton::update()
{
    glm::mat4 temp = glm::mat4(1.0f);
	root->Update(temp);
}

// ○ Rendering everything through draw()

void Skeleton::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	root->Draw(viewProjMtx, shader);
}



void Skeleton::BuildJointVector() {
    CollectJoints(root); // Recursively collect joints
}

void Skeleton::CollectJoints(Joint* node) {
    if (!node) return;
    
    joints.push_back(node); // Add this joint to the vector

    // Recursively collect children
    for (Joint* child : node->children) {
        CollectJoints(child);
    }
}

glm::mat4 Skeleton::GetWorldMatrix(int jointIndex) {
    if (jointIndex < 0 || jointIndex >= joints.size()) {
        std::cerr << "Error: Joint index out of range!" << std::endl;
        return glm::mat4(1.0f); // Return identity matrix as fallback
    }
    return joints[jointIndex]->W; // Return world matrix
}
