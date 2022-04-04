#include <SDL2/SDL.h>

#ifndef DEBUG_H

#define DEBUG_H

extern SDL_Texture *screenshots[3];
extern uint8_t screenshot;
extern SDL_bool grid;

void initDebug(void);
void renderDebugText(void);
void renderGrid(void);
void renderScreenshot(void);

#endif

// EOF
