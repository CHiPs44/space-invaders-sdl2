#include <SDL2/SDL.h>

#include "../include/items.h"
#include "../include/font.h"
#include "../include/graphics.h"
#include "../include/player.h"
#include "../include/scene.h"

uint16_t scores[3] = {6789, 1010, 2020}; // HI, P1, P2
uint8_t lives = 3;
uint8_t credits = 9;
SDL_bool lineFlag = SDL_FALSE;
SDL_bool livesFlag = SDL_FALSE;

/**
 * @brief Render player one, high, player two scores
 */
void renderScores(void)
{
    renderText("SCORE<1> HI-SCORE SCORE<2>", 1, 1 * 8);
    for (uint8_t i = 0; i < 3; i += 1)
    {
        // 0=HI, 1=P1, 2=P2
        // When playing, don't display P2 when P1 is playing & vice-versa
        if (scene != SCENE_GAME || i == 0 || (scene == SCENE_GAME && player == i))
        {
            //           1         2
            // 012345678901234567890123456789
            //  SCORE<1> HI-SCORE SCORE<2>
            //    0000    0000      0000   <- original
            //    0000     0000     0000   <- my version
            int column = i == 0 ? 12 : (i == 1 ? 3 : 21);
            renderNumber(scores[i] % 10000, 4, column, 3 * 8);
        }
    }
}

/**
 * @brief Render lives
 */
void renderLives(void)
{
    if (!livesFlag)
        return;
    renderChar('0' + lives, 1, 30 * 8);
    for (uint8_t i = 0; i < lives - 1; i += 1)
    {
        SDL_Rect rect = {(26 + i * 16) * zoom, 30 * 8 * zoom,
                         SHIP_WIDTH * zoom, SHIP_HEIGHT * zoom};
        SDL_RenderCopy(renderer, ship->texture, NULL, &rect);
    }
}

/**
 * @brief Render credits
 */
void renderCredits(void)
{
    renderText("CREDIT", 17, 30 * 8);
    renderNumber(credits, 2, 24, 30 * 8);
}

/**
 * @brief Render line between player's ship and left lives / credits
 */
void renderLine(void)
{
    if (!lineFlag)
        return;
    SDL_Rect lineRect = {0 * zoom, 239 * zoom, GAME_WIDTH * zoom, 1 * zoom};
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &lineRect);
}

// EOF
