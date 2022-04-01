#include "../include/saucer.h"
#include "../include/sprite.h"
#include "../include/graphics.h"

Sprite *saucer = NULL;
SDL_bool flagSaucer = SDL_FALSE;

void initSaucer()
{
    saucer = createSprite1(renderer, "saucer.png", 2);
    saucer->rect.y = SAUCER_Y;
}

void renderSaucer()
{
    if (!flagSaucer)
        return;
    saucer->frame = 0;
    if (saucerExploding > 0)
    {
        if (saucerExploding < SDL_GetTicks())
        {
            saucer->frame = 1;
        }
        else
        {
            saucerExploding = 0;
        }
    }
    renderSprite(saucer, renderer, zoom);
}

// EOF
