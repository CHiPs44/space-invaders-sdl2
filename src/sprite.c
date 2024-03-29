/**
 * @brief Sprite "library"
 */

#include "../include/sprite.h"
#include "../include/graphics.h"

/**
 * @brief Further initialize a sprite object
 *
 * @param sprite
 * @param frames
 * @return Sprite*
 */
Sprite *createSprite0(Sprite *sprite, int frames)
{
    int w, h;
    SDL_QueryTexture(sprite->texture, NULL, NULL, &w, &h);
    sprite->frames = frames;
    sprite->frame = 0;
    sprite->rect.w = w;
    sprite->rect.h = h / frames;
    sprite->rect.x = 0;
    sprite->rect.y = 0;
    return sprite;
}

/**
 * @brief Create a sprite object with a image file
 *
 * @param filename File name, e.g. "ship.png"
 * @param frames   Frame count (vertical)
 * @return Sprite*
 */
Sprite *createSpriteFromFile(char *filename, int frames)
{
    Sprite *sprite = malloc(sizeof(Sprite));
    char buffer[256];
    snprintf(buffer, 256, "./assets/%s", filename);
    sprite->texture = IMG_LoadTexture(renderer, buffer);
    return createSprite0(sprite, frames);
}

/**
 * @brief Create sprite object from SDL texture
 *
 * @param texture SDL texture
 * @param frames Frame count (vertical)
 * @return Sprite*
 */
Sprite *createSpriteFromTexture(SDL_Texture *texture, int frames)
{
    Sprite *sprite = malloc(sizeof(Sprite));
    sprite->texture = texture;
    return createSprite0(sprite, frames);
}

void killSprite(Sprite *sprite)
{
    if (sprite == NULL)
        return;
    if (sprite->texture != NULL)
        SDL_DestroyTexture(sprite->texture);
    free(sprite);
}

/**
 * @brief Render sprite
 *
 * @param sprite Sprite object
 */
void renderSprite(Sprite *sprite)
{
    // SDL_assert(NULL!=sprite);
    int frame = sprite->frame < 0
                    ? 0
                    : (sprite->frame > sprite->frames ? sprite->frames : sprite->frame);
    SDL_Rect source = {
        0, sprite->rect.h * frame,
        sprite->rect.w, sprite->rect.h};
    SDL_Rect destination = {
        offsetX + sprite->rect.x * zoom,
        offsetY + sprite->rect.y * zoom,
        sprite->rect.w * zoom,
        sprite->rect.h * zoom};
    if (0 != SDL_RenderCopy(renderer, sprite->texture, &source, &destination))
    {
        fprintf(stderr, "SDL_RenderCopy!");
    }
}

/**
 * @brief Is sprite 1 intersecting with sprite 2?
 */
SDL_bool isIntersectingSprite(Sprite *sprite1, Sprite *sprite2)
{
    SDL_Rect result;
    return SDL_IntersectRect(&sprite1->rect, &sprite2->rect, &result);
}

// EOF
