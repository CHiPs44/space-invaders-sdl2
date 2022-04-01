#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef GRAPHICS_H

#define GRAPHICS_H

#define GAME_WIDTH 224
#define GAME_HEIGHT 256

extern int zoom;
extern SDL_Window *window;
extern SDL_Renderer *renderer;

void graphicsQuit(int status, char *message);
void graphicsInit();

#endif

// EOF
