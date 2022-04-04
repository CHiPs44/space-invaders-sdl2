/**
 * @brief Sprite "library"
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef SPRITE_H

#define SPRITE_H

typedef struct
{
    SDL_Texture *texture;
    int frames;
    int frame;
    SDL_Rect rect;
} Sprite;

extern Sprite *createSprite0(Sprite *sprite, int frames);
extern Sprite *createSprite1(char *filename, int frames);
extern Sprite *createSprite2(SDL_Texture *texture, int frames);
extern void killSprite(Sprite *sprite);
extern void renderSprite(Sprite *sprite);

#endif

// EOF
