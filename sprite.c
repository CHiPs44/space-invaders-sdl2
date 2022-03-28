/**
 * @brief Sprite "library"
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>

typedef struct
{
    SDL_Texture *texture;
    int frames;
    int frame;
    // int x;
    // int y;
    // int w;
    // int h;
    SDL_Rect rect;
} Sprite;

Sprite *createSprite(SDL_Renderer *renderer, char *filename, int frames)
{
    Sprite *sprite = malloc(sizeof(Sprite));
    sprite->texture = IMG_LoadTexture(renderer, filename);
    int w, h;
    SDL_QueryTexture(sprite->texture, NULL, NULL, &w, &h);
    sprite->frames = frames;
    sprite->frame = 0;
    sprite->rect.w = w;
    sprite->rect.h = h / frames;
    sprite->rect.x = 0;
    sprite->rect.y = 0;
}

int renderSprite(Sprite *sprite, SDL_Renderer *renderer, int zoom)
{
    SDL_Rect source = {
        0, sprite->rect.h * sprite->frame,
        sprite->rect.w, sprite->rect.h};
    SDL_Rect destination = {
        sprite->rect.x * zoom, sprite->rect.y * zoom,
        sprite->rect.w * zoom, sprite->rect.h * zoom};
    return SDL_RenderCopy(
        renderer,
        sprite->texture,
        &source,
        &destination);
}

// EOF
