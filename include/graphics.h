#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef GRAPHICS_H

#define GRAPHICS_H

#define STATUS_RESET -1

#define GAME_WIDTH 224
#define GAME_HEIGHT 256

#define WINDOW_WIDTH 240
#define WINDOW_HEIGHT 320

#define DEBUG_HEIGHT 16

#define WITH_SCREENSHOTS SDL_TRUE

extern int zoom;
extern int offsetX;
extern int offsetY;
extern SDL_Window *window;
extern SDL_Renderer *renderer;

void stopGraphics(int status, const char *message, const char *error);
void initGraphics(void);

#endif

// EOF
