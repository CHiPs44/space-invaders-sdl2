/**
 * @brief Sprite "library"
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>

#ifndef SPRITE_H

#define SPRITE_H

typedef struct
{
    SDL_Texture *texture;
    int frames;
    int frame;
    SDL_Rect rect;
} Sprite;

Sprite *createSprite0(Sprite *sprite, int frames);
Sprite *createSprite1(SDL_Renderer *renderer, char *filename, int frames);
Sprite *createSprite2(SDL_Renderer *renderer, SDL_Texture *texture, int frames);
void killSprite(Sprite *sprite);
void renderSprite(Sprite *sprite, SDL_Renderer *renderer, int zoom);

#endif

// EOF
