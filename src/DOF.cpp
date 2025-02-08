#include "DOF.h"
#include <algorithm> // For std::max and std::min

// Constructor for DOF
DOF::DOF() {
    value = 0.0f;  // Initial value is set to 0 (no rotation by default)
    minLimit = -100000.0f;
    maxLimit = 100000.0f;
}

// Set the value of the DOF, clamping it within the specified limits
void DOF::SetValue(float val) {
    if ((val <= maxLimit) && (minLimit <= val)) value = val;
    else if (val < minLimit) value = minLimit;
    else if (maxLimit < val) value = maxLimit;
}



// Get the current value of the DOF (rotation angle)
float DOF::GetValue()  {
    return value;
}

// Set the minimum and maximum rotation limits for the DOF
void DOF::SetMinMax(float minVal, float maxVal) {
    minLimit = minVal;
    maxLimit = maxVal;
}

// Optional: Get the minimum limit for the DOF
float DOF::GetMinLimit()  {
    return minLimit;
}

// Optional: Get the maximum limit for the DOF
float DOF::GetMaxLimit()  {
    return maxLimit;
}
