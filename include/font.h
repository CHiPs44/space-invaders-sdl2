#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef FONT_H

#define FONT_H

extern SDL_Texture *font;

void initFont();
void renderChar(char c, uint8_t column, uint16_t y);
void renderText(char *text, uint8_t column, uint8_t y);
void renderNumber(uint16_t number, uint8_t width, uint8_t column, uint8_t y);

#endif

// EOF
