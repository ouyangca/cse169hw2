#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Tokenizer.h"
#include "DOF.h"
#include "Cube.h"

class Joint {
public:
    Joint();
    ~Joint();

    bool Load(Tokenizer& t);                      // Loading function (corrected)
    void AddChild(Joint* child);                  // Add child joint to this joint
    void Update(glm::mat4& parent);               // Update this joint's transformations
    void Draw(const glm::mat4& viewProjMtx, GLuint shader);  // Draw this joint (updated signature)
    void PrintValues() const;
    std::vector<Joint*> children;                // Child joints
    glm::mat4 L, W;                              // Local and world matrices
    glm::mat4 inverseB;
    char JointName[256];                         // Name of the joint




private:
    glm::vec3 offset;                            // Offset vector (M)
    glm::vec3 boxmin, boxmax;                    // Bounding box min and max
    glm::vec3 pose;
    Cube* cube;                                  // Cube for visualization
    std::vector<DOF*> JointDOF;                  // Degrees of freedom (DOFs) for the joint

    // char JointName[256];                         // Name of the joint
};
