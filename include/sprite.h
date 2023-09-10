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

Sprite *createSprite0(Sprite *sprite, int frames);
Sprite *createSpriteFromFile(char *filename, int frames);
Sprite *createSpriteFromTexture(SDL_Texture *texture, int frames);
void killSprite(Sprite *sprite);
void renderSprite(Sprite *sprite);
SDL_bool isIntersectingSprite(Sprite *sprite1, Sprite *sprite2);

#endif

// EOF
