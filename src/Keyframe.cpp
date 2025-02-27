#include "Keyframe.h"
#include <iostream>

// Constructor implementation
Keyframe::Keyframe(){
}
Keyframe::~Keyframe() {
    // If necessary, add cleanup code here
}


// Load function implementation
bool Keyframe::Load(Tokenizer* tokenizer) {
    // Load keyframes
    float key_time = tokenizer->GetFloat();
    float key_value = tokenizer->GetFloat();

    tokenizer->SkipWhitespace();
    char tanType = tokenizer->CheckChar();
    if (97 <= tanType && tanType <= 142) {
        tokenizer->GetToken(ruleIn);
        tokenizer->GetToken(ruleOut);
    } else {
        inTangent = tokenizer->GetFloat();
        outTangent = tokenizer->GetFloat();
    }

    time = key_time;
    value = key_value;

    return true;
}

// Compute cubic coefficients implementation
void Keyframe::computeCubicCoefficients() {
    a = 2 * value - 2 * value + inTangent + outTangent;
    b = -3 * value + 3 * value - 2 * inTangent - outTangent;
    c = inTangent;
    d = value;
}

// Interpolate function implementation
float Keyframe::interpolate(float t) const {
    float t2 = t * t;
    float t3 = t2 * t;
    return a * t3 + b * t2 + c * t + d;
}
