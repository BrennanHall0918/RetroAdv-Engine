#include "app.h"

void log_error(char *prefix) { SDL_Log("%s: %s", prefix, SDL_GetError()); }