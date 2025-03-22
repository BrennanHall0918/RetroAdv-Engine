#include "app.h"

void log_error(char *prefix) { SDL_Log("%s: %s", prefix, SDL_GetError()); }

float clamp (float n, float min, float max) 
{ 
    if (n < min) {
        n = min;
    }

    if (n > max) {
        n = max;
    }

    return n;
}