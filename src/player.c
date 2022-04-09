#include "../include/player.h"
#include "../include/const.h"
#include "../include/graphics.h"
#include "../include/scene.h"

Sprite *ship = NULL;
Sprite *shoot = NULL;
Sprite *shootExplosion = NULL;
SDL_bool shipFlag = SDL_FALSE;
SDL_bool shootFlag = SDL_FALSE;
uint8_t player = 0;
int8_t shipDx = 0;
uint32_t playerExploding = 0L;
uint32_t shootExploding = 0L;

void initPlayer(void)
{
    ship = createSpriteFromFile("ship.png", 1);
    ship->rect.y = SHIP_Y;
    shoot = createSpriteFromFile("shoot.png", 1);
    shootExplosion = createSpriteFromFile("shoot-explosion.png", 1);
    shootExplosion->rect.y = 4 * 8 - 8;
}

/**
 * @brief Render ship
 */
void renderShip(void)
{
    if (!shipFlag)
        return;
    renderSprite(ship);
}

/**
 * @brief Render shoot
 */
void renderShoot(void)
{
    if (!shootFlag)
        return;
    if (shootExploding != 0L)
        renderSprite(shoot);
    else
        renderSprite(shootExplosion);
}

// EOF
