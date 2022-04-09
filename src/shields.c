#include "../include/shields.h"
#include "../include/graphics.h"
#include "../include/sprite.h"

SDL_bool shieldsFlag = SDL_FALSE;
Sprite *shields[4] = {NULL, NULL, NULL, NULL};

void initShields(void)
{
    for (int i = 0; i < 4; i += 1)
    {
        killSprite(shields[i]);
        // one sprite/texture for each shield
        shields[i] = createSpriteFromFile("shield.png", 1);
        // TODO find exact location (32 is good for #1)
        shields[i]->rect.x = 32 + i * 40;
        shields[i]->rect.y = SHIELD_Y;
    }
}

void renderShields(void)
{
    if (!shieldsFlag)
        return;
    for (int i = 0; i < 4; i += 1)
    {
        renderSprite(shields[i]);
    }
}

void shootAtShield(int bombType, uint8_t x)
{
    int foo = bombType;
    foo += x;
    // TODO check if shield touched and destroy some pixels in the texture
    // bombType:
    //  0=>player shoot
    //  1..3=> alien bombs
}

// EOF
