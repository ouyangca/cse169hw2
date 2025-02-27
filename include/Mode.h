#include <cstring>



#ifndef MODE_H
#define MODE_H

enum Mode {
    CONSTANT,
    LINEAR,
    CYCLE,
    CYCLE_OFFSET,
    BOUNCE,
    UNKNOWN  // Default case for invalid inputs
};

// Function declaration
Mode getMode(const char* mode);

#endif  // MODE_H
