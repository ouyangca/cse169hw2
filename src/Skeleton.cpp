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

void Skeleton::Update(glm::mat4 parentW)
{
	root->Update(parentW);
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

void Skeleton::PrintDOF() {
    for (int jointIndex = 0; jointIndex < joints.size(); jointIndex++) {
        std::cout << "Joint Index: " << jointIndex << "\n";
        joints[jointIndex]->PrintValues();
    }
    
}



void Skeleton::SetDOF(int jointIndex, int dofIndex, float newValue) {
    if (jointIndex < 0 || jointIndex >= joints.size()) {
        std::cerr << "Invalid joint index!" << std::endl;
        return;
    }

    Joint* joint = joints[jointIndex];

    if (dofIndex < 0 || dofIndex >= joint->JointDOF.size()) {
        std::cerr << "Invalid DOF index!" << std::endl;
        return;
    }

    joint->JointDOF[dofIndex]->SetValue(newValue);
    std::cout << "Updated DOF[" << dofIndex << "] of Joint[" << jointIndex << "] to " << newValue << std::endl;
}
