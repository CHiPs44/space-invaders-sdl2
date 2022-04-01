#include "../include/debug.h"

#include "../include/aliens.h"
#include "../include/font.h"
#include "../include/graphics.h"
#include "../include/items.h"
#include "../include/player.h"
#include "../include/saucer.h"
#include "../include/scene.h"

SDL_Texture *screenshots[3];
uint8_t screenshot = 2;
SDL_bool grid = SDL_FALSE;

void initDebug(void)
{
    screenshots[0] = IMG_LoadTexture(renderer, "./intro1.png");
    screenshots[1] = IMG_LoadTexture(renderer, "./player-1.png");
    screenshots[2] = IMG_LoadTexture(renderer, "./game-1.png");
}

/**
 * @brief Render debug line under game & screenshot
 *
 * @param sceneName BOOT/HOME/PLAY/GAME/...
 * @param duration  since last scene change
 */
void renderDebugText(char *sceneName, uint32_t duration)
{
    char flags[] = "lsfax-";
    if (flagLine)
        flags[0] = 'L';
    if (flagShip)
        flags[1] = 'S';
    if (flagShoot)
        flags[2] = 'F';
    if (flagAliens)
        flags[3] = 'A';
    if (flagSaucer)
        flags[4] = 'X';
    if (saucerExploding)
        flags[5] = '*';
    char buffer[256];
    sprintf(buffer,
            "%s %08d %s SHIP:%03d SHOOT:%03d SPD/DLY:%02d/%02d %d",
            sceneName, duration, flags,
            ship->rect.x, shoot->rect.y,
            speed, delay, saucer->frame);
    uint8_t i = 0;
    while (buffer[i])
    {
        renderChar(buffer[i], 1 + i, GAME_HEIGHT);
        i += 1;
    }
}

void renderGrid(void)
{
    if (!grid)
        return;
    for (uint16_t x = 0; x < GAME_WIDTH * 2; x += 8)
    {
        if (x > GAME_WIDTH)
        {
            SDL_SetRenderDrawColor(renderer, 0xc0, 0xc0, 0xc0, SDL_ALPHA_OPAQUE);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, SDL_ALPHA_OPAQUE);
        }
        for (uint16_t y = 0; y < GAME_HEIGHT * 2; y += 8)
        {
            SDL_Rect rect = {
                x * zoom, y * zoom,
                (x + 8) * zoom, (y + 8) * zoom};
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}

void renderScreenshot(void)
{
    SDL_Rect screenshotRect = {GAME_WIDTH * zoom, 0, GAME_WIDTH * zoom, GAME_HEIGHT * zoom};
    SDL_RenderCopy(renderer, screenshots[screenshot], NULL, &screenshotRect);
}

// EOF
