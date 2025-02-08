#pragma once

#include <vector>
#include "core.h"

class DOF {
    
private:
    float value;         // Current value (rotation angle)
    float minLimit;      // Minimum limit for rotation (in radians)
    float maxLimit;      // Maximum limit for rotation (in radians)

public:
    DOF();
    ~DOF();

    // Set the value of the DOF, clamping it within the defined limits
    void SetValue(float val);

    // Get the current value (rotation angle) of the DOF
    float GetValue() ;

    // Set the minimum and maximum limits for the DOF
    void SetMinMax(float minVal, float maxVal);

    // Get the minimum limit for the DOF
    float GetMinLimit() ;

    // Get the maximum limit for the DOF
    float GetMaxLimit() ;
    
};