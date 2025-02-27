#include "Mode.h"
#include <cstring>  // For strcmp

Mode getMode(const char* mode) {
    if (strcmp(mode, "constant") == 0) {
        return CONSTANT;
    } else if (strcmp(mode, "linear") == 0) {
        return LINEAR;
    } else if (strcmp(mode, "cycle") == 0) {
        return CYCLE;
    } else if (strcmp(mode, "cycle_offset") == 0) {
        return CYCLE_OFFSET;
    } else if (strcmp(mode, "bounce") == 0) {
        return BOUNCE;
    } else {
        return UNKNOWN;
    }
}
