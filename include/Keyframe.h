#pragma once
#include <vector>
#include "Tokenizer.h"
#include "Mode.h"
#include <glm/glm.hpp>

class Keyframe {
public:
    float time;       // Timestamp of the keyframe
    float value;      // Value at this keyframe
    float inTangent;  // Slope of the curve entering the keyframe: fix tangent mode 
    float outTangent; // Slope of the curve exiting the keyframe: fix tangent mode

    // Cubic coefficients for smooth interpolation
    float a, b, c, d;
    // 
    char ruleIn[256], ruleOut[256]; // tangent mode

    // Constructor
    Keyframe();
    ~Keyframe();

    // Member functions
    bool Load(class Tokenizer* tokenizer);
    void computeCubicCoefficients();
    float interpolate(float t) const;
};