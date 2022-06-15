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
SDL_bool gridVisible = SDL_FALSE;
SDL_bool debugVisible = SDL_TRUE;
SDL_bool screenshotVisible = SDL_FALSE;

void initDebug(void)
{
    screenshots[0] = IMG_LoadTexture(renderer, "./assets/intro1.png");
    screenshots[1] = IMG_LoadTexture(renderer, "./assets/player-1.png");
    screenshots[2] = IMG_LoadTexture(renderer, "./assets/game-1.png");
}

/**
 * @brief Render debug line under game & screenshot
 *
 * @param sceneName BOOT/HOME/PLAY/GAME/...
 * @param duration  since last scene change
 */
void renderDebugLine(int line, char *buffer)
{
    uint8_t i = 0;
    while (buffer[i])
    {
        renderChar(buffer[i], 1 + i, GAME_HEIGHT + 8 + 8 * line);
        i += 1;
    }
}

/**
 * @brief Render debug text under game & screenshot
 *
 * @param sceneName BOOT/HOME/PLAY/GAME/...
 * @param duration  since last scene change
 */
void renderDebugText(void)
{
    if (!debugVisible)
        return;
    char buffer[256];
    snprintf(buffer, 256,
             "S:%s D:%06.01f F:%06d",
             sceneName, sceneDuration / 1000.0, frames);
    renderDebugLine(0, buffer);
    snprintf(buffer, 256,
             "M:%02d S:%03d D:%03d",
             sceneDuration > 0 ? frames * 1000 / sceneDuration : 0, speed, delay);
    renderDebugLine(1, buffer);
}

void renderGrid(void)
{
    if (!gridVisible)
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
                offsetX + x * zoom, offsetY + y * zoom,
                (x + 8) * zoom, (y + 8) * zoom};
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}

void renderScreenshot(void)
{
    if (!screenshotVisible)
        return;
    SDL_Rect screenshotRect =
        {offsetX + GAME_WIDTH * zoom, offsetY + 0 * zoom,
         GAME_WIDTH * zoom, GAME_HEIGHT * zoom};
    SDL_RenderCopy(renderer, screenshots[screenshot], NULL, &screenshotRect);
}

// EOF
