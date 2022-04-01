#include "../include/player.h"
#include "../include/const.h"
#include "../include/graphics.h"
#include "../include/scene.h"

Sprite *ship = NULL;
Sprite *shoot = NULL;
Sprite *shootExplosion = NULL;
SDL_bool flagShip = SDL_FALSE;
SDL_bool flagShoot = SDL_FALSE;
uint8_t player = 0;
int8_t shipDx = 0;
uint32_t playerExploding = 0L;
uint32_t shootExploding = 0L;

void initPlayer(void)
{
    ship = createSprite1(renderer, "ship.png", 1);
    ship->rect.y = SHIP_Y;
    shoot = createSprite1(renderer, "shoot.png", 1);
    shootExplosion = createSprite1(renderer, "shoot-explosion.png", 1);
    shootExplosion->rect.y = 4 * 8 - 8;
}

/**
 * @brief Render ship
 */
void renderShip(void)
{
    if (!flagShip)
        return;
    renderSprite(ship, renderer, zoom);
}

/**
 * @brief Render shoot
 */
void renderShoot(void)
{
    if (!flagShoot)
        return;
    if (shootExploding != 0L)
        renderSprite(shoot, renderer, zoom);
    else
        renderSprite(shootExplosion, renderer, zoom);
}

// EOF
