
#include "Channel.h"
#include <cstring> // For strcpy
#include <iostream>


// Constructor
Channel::Channel(){
    // Initialize extrapolation modes to default values
    // strcpy(extrap_in, "constant");
    // strcpy(extrap_out, "constant");
}

// Destructor
Channel::~Channel() {
    // Clean up dynamically allocated keyframes
    for (auto keyframe : keyframes) {
        delete keyframe;
    }
    keyframes.clear();
}

// Load channel data from a tokenizer
bool Channel::Load(Tokenizer* tokenizer) {
    // Extrapolation mode: constant, linear, cycle, cycle_offset, bounce
    tokenizer->FindToken("extrapolate");

    char in_mode[256], out_mode[256];
    tokenizer->GetToken(extrap_in);
    tokenizer->GetToken(extrap_out);

    // strcpy(extrap_in, in_mode);
    // strcpy(extrap_out, out_mode);

    // Keys
    tokenizer->FindToken("keys");
    numKeys = tokenizer->GetFloat();

    tokenizer->FindToken("{");
    for (int i = 0; i < numKeys; i++) {
        Keyframe* keyframe = new Keyframe();
        if (keyframe->Load(tokenizer)) {
            keyframes.push_back(keyframe);
        } else {
            delete keyframe; // Clean up if loading fails
            std::cout << "ERROR loading Channel" << std::endl;
            return false;
        }
    }

    return true;
}


// Linear interpolation between two keyframes
float Channel::interpolate(const Keyframe& k1, const Keyframe& k2, float time) {
    float t = (time - k1.time) / (k2.time - k1.time);
    return k1.value + t * (k2.value - k1.value);
}


// Precompute tangentIn & tangentOut for each key
void Channel::Precompute() {
    if (numKeys == 1) {
        keyframes[0]->inTangent = 0;
        keyframes[0]->outTangent = 0;
        return;
    }

    // Compute tangents for each keyframe
    for (int i = 0; i < numKeys; i++) {
        if (strcmp(keyframes[i]->ruleIn, "flat") == 0) {
            keyframes[i]->inTangent = 0;
        } else if (strcmp(keyframes[i]->ruleIn, "linear") == 0) {
            if (i == 0) {
                keyframes[i]->inTangent = (keyframes[i + 1]->value - keyframes[i]->value) / (keyframes[i + 1]->time - keyframes[i]->time);
            } else {
                keyframes[i]->inTangent = (keyframes[i]->value - keyframes[i - 1]->value) / (keyframes[i]->time - keyframes[i - 1]->time);
            }
        } else if (strcmp(keyframes[i]->ruleIn, "smooth") == 0) {
            if (i == 0) {
                keyframes[i]->inTangent = (keyframes[i + 1]->value - keyframes[i]->value) / (keyframes[i + 1]->time - keyframes[i]->time);
            } else if (i == numKeys - 1) {
                keyframes[i]->inTangent = (keyframes[i]->value - keyframes[i - 1]->value) / (keyframes[i]->time - keyframes[i - 1]->time);
            } else {
                keyframes[i]->inTangent = (keyframes[i + 1]->value - keyframes[i - 1]->value) / (keyframes[i + 1]->time - keyframes[i - 1]->time);
            }
        }
        

        // Compute tanOut from ruleOut ("flat", "linear", "smooth")
        if (strcmp(keyframes[i]->ruleOut, "flat") == 0) {
            keyframes[i]->outTangent = 0;
        } else if (strcmp(keyframes[i]->ruleOut, "linear") == 0) {
            if (i == numKeys - 1) {
                keyframes[i]->outTangent = (keyframes[i]->value - keyframes[i - 1]->value) / (keyframes[i]->time - keyframes[i - 1]->time);
            } else {
                keyframes[i]->outTangent = (keyframes[i + 1]->value - keyframes[i]->value) / (keyframes[i + 1]->time - keyframes[i]->time);
            }
        } else if (strcmp(keyframes[i]->ruleOut, "smooth") == 0) {
            if (i == 0) {
                keyframes[i]->outTangent = (keyframes[i + 1]->value - keyframes[i]->value) / (keyframes[i + 1]->time - keyframes[i]->time);
            } else if (i == numKeys - 1) {
                keyframes[i]->outTangent = (keyframes[i]->value - keyframes[i - 1]->value) / (keyframes[i]->time - keyframes[i - 1]->time);
            } else {
                keyframes[i]->outTangent = (keyframes[i + 1]->value - keyframes[i - 1]->value) / (keyframes[i + 1]->time - keyframes[i - 1]->time);
            }
        }
    }

    // Precompute cubic coefficients for each keyframe
    for (int i = 0; i < numKeys - 1; i++) {
        glm::vec4 g;
        g.x = keyframes[i]->value;
        g.y = keyframes[i + 1]->value;
        g.z = (keyframes[i + 1]->time - keyframes[i]->time) * keyframes[i]->outTangent;
        g.w = (keyframes[i + 1]->time - keyframes[i]->time) * keyframes[i + 1]->inTangent;

        keyframes[i]->a = glm::dot(glm::vec4(2.0f, -2.0f, 1.0f, 1.0f), g);
        keyframes[i]->b = glm::dot(glm::vec4(-3.0f, 3.0f, -2.0f, -1.0f), g);
        keyframes[i]->c = g.z;
        keyframes[i]->d = g.x;
    }
}




// Evaluate the channel at a given time
float Channel::Evaluate(float time) {
    
    float evalValue = 0;
    float duration = keyframes[numKeys - 1]->time - keyframes[0]->time; 
    float deltaVal = keyframes[numKeys - 1]->value - keyframes[0]->value; 



    for (int i = 0; i < numKeys; i++) {
        if (time == keyframes[i]->time) {
            return keyframes[i]->value;
        }
    }

    if (time < keyframes[0]->time) {
        if (strcmp(extrap_in, "constant") == 0) {
            evalValue = keyframes[0]->value;
        } else if (strcmp(extrap_in, "linear") == 0) {
            evalValue = keyframes[0]->value - keyframes[0]->inTangent * (keyframes[0]->time - time);
        } else if (strcmp(extrap_in, "cycle") == 0) {
            evalValue = Evaluate(time + duration);
        } else if (strcmp(extrap_in, "cycle_offset") == 0) {
            evalValue = Evaluate(time + duration) - deltaVal;
        } else if (strcmp(extrap_in, "bounce") == 0) {
            int numCycles = ComputeCycleNum(time, keyframes[0]->time, duration);
            if (numCycles % 2 == 1) {
                evalValue = Evaluate(time + duration * (numCycles + 1));
            } else {
                evalValue = Evaluate(2 * keyframes[0]->time - (time + duration * numCycles));
            }
        }
    }

    else if (time > keyframes[numKeys - 1]->time) {
        if (strcmp(extrap_out, "constant") == 0) {
            evalValue = keyframes[numKeys - 1]->value;
        } else if (strcmp(extrap_out, "linear") == 0) {
            evalValue = keyframes[numKeys - 1]->value + keyframes[numKeys - 1]->outTangent * (time - keyframes[numKeys - 1]->time);
        } else if (strcmp(extrap_out, "cycle") == 0) {
            evalValue = Evaluate(time - duration);
        } else if (strcmp(extrap_out, "cycle_offset") == 0) {
            evalValue = Evaluate(time - duration) + deltaVal;
        } else if (strcmp(extrap_out, "bounce") == 0) {
            int numCycles = ComputeCycleNum(time, keyframes[numKeys - 1]->time, duration);
            if (numCycles % 2 == 1) {
                evalValue = Evaluate(time - duration * (numCycles + 1));
            } else {
                evalValue = Evaluate(2 * keyframes[numKeys - 1]->time - (time - duration * numCycles));
            }
        }
    }

    else {
        int left = 0, right = numKeys - 1;
        while (right - left > 1) {
            int mid = left + (right - left) / 2;
            if (time < keyframes[mid]->time) {
                right = mid;
            } else {
                left = mid;
            }
        }
        float u = (time - keyframes[left]->time) / (keyframes[right]->time - keyframes[left]->time);
        evalValue = keyframes[left]->d + u * (keyframes[left]->c + u * (keyframes[left]->b + u * keyframes[left]->a));
    }

    return evalValue;
}



int Channel::ComputeCycleNum(float curTime, float keyTime, float duration) {
    int count = 0;
    if (curTime < keyTime) {
        while (curTime < keyTime) {
            curTime += duration;
            count++;
        }
        count--; 
    } else {
        while (curTime > keyTime) {
            curTime -= duration;
            count++;
        }
        count--;
    }
    return count;
}