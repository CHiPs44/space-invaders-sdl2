/*
 * Scene management
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../include/graphics.h"
#include "../include/scene.h"
#include "../include/items.h"
#include "../include/player.h"
#include "../include/aliens.h"
#include "../include/shields.h"
#include "../include/saucer.h"

int32_t speed = 7;
int32_t delay;

uint8_t scene = SCENE_NONE;
uint32_t sceneStart = 0L;
uint32_t sceneDuration = 0L;
uint32_t step = 0L;

uint8_t playMode = 0; // 1=1 player, 2=2 players
int saucerExploding = 0L;

/**
 * @brief Set the scene
 *
 * @param newScene SCENE_XXXX
 */
void setScene(uint8_t newScene)
{
    flagLine = SDL_FALSE;
    flagLives = SDL_FALSE;
    flagShip = SDL_FALSE;
    flagShoot = SDL_FALSE;
    flagAliens = SDL_FALSE;
    flagSaucer = SDL_FALSE;
    flagShields = SDL_FALSE;

    switch (newScene)
    {
    case SCENE_BOOT:
        // display PLAY SPACE INVADERS
        scene = newScene;
        sceneStart = SDL_GetTicks();
        break;
    case SCENE_HOME:
        // display "instructions" & aliens points
        scene = newScene;
        sceneStart = SDL_GetTicks();
        break;
    case SCENE_PLAY:
        // start 1 player game (for now)
        scene = newScene;
        sceneStart = SDL_GetTicks();
        playMode = 1;
        player = 1;
        break;
    case SCENE_GAME:
        // efectively play game
        scene = newScene;
        sceneStart = SDL_GetTicks();
        ship->rect.x = (GAME_WIDTH - SHIP_WIDTH) / 2;
        shipDx = 0;
        flagLine = SDL_TRUE;
        flagLives = SDL_TRUE;
        flagShip = SDL_TRUE;
        flagAliens = SDL_TRUE;
        flagShields = SDL_TRUE;
        break;
    default:
        fprintf(stderr, "Unknown scene #%d\n", newScene);
        break;
    }
}

void renderScene()
{
    static uint32_t lastTicks = 0;
    uint32_t ticks = SDL_GetTicks();
    sceneDuration = ticks - sceneStart;

    // Clear screen to black
    SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    // Render "always here" items, except in BOOT scene
    if (scene != SCENE_BOOT)
    {
        renderScores();
        renderLives();
        renderCredits();
    }

    switch (scene)
    {
    case SCENE_BOOT:
        renderDebugText("BOOT", sceneDuration);
        // Switch to HOME scene after 1 second
        if (sceneDuration > 1000)
        {
            setScene(SCENE_HOME);
        }
        else
        {
            for (uint8_t line = 0; line < GAME_HEIGHT / 8; line++)
            {
                for (uint8_t col = 0; col < GAME_WIDTH / 8; col++)
                {
                    int c = rand() % (48 + 2 * sceneDuration);
                    if (c <= 255)
                    {
                        renderChar(c > 127 ? 127 : c, col, line * 8);
                    }
                }
            }
        }
        break;
    case SCENE_HOME:
        renderDebugText("HOME", sceneDuration);
        // WIP!
        renderText("PLAY", 12, 8 * 8);
        renderText("SPACE  INVADERS", 7, 11 * 8);
        renderText("*SCORE ADVANCE TABLE*", 3, 15 * 8);
        SDL_Rect sourceS = {0, 0, SAUCER_WIDTH, SAUCER_HEIGHT};
        SDL_Rect destinationS = {(9 - 2) * 8 * zoom, 17 * 8 * zoom, SAUCER_WIDTH * zoom, SAUCER_HEIGHT * zoom};
        SDL_RenderCopy(renderer, saucer->texture, &sourceS, &destinationS);
        renderText("=? MYSTERY", 9, 17 * 8);

        SDL_Rect source3 = {0, 0, ALIEN3_WIDTH, ALIEN3_HEIGHT};
        SDL_Rect destination3 = {(9 - 2) * 8 * zoom, 19 * 8 * zoom, ALIEN3_WIDTH * zoom, ALIEN3_HEIGHT * zoom};
        SDL_RenderCopy(renderer, alien3->texture, &source3, &destination3);
        renderText("=30 POINTS", 9, 19 * 8);

        SDL_Rect source2 = {0, 0, ALIEN2_WIDTH, ALIEN2_HEIGHT};
        SDL_Rect destination2 = {(9 - 2) * 8 * zoom, 21 * 8 * zoom, ALIEN2_WIDTH * zoom, ALIEN2_HEIGHT * zoom};
        SDL_RenderCopy(renderer, alien2->texture, &source2, &destination2);
        renderText("=20 POINTS", 9, 21 * 8);

        SDL_Rect source1 = {0, 0, ALIEN1_WIDTH, ALIEN1_HEIGHT};
        SDL_Rect destination1 = {(9 - 2) * 8 * zoom, 23 * 8 * zoom, ALIEN1_WIDTH * zoom, ALIEN1_HEIGHT * zoom};
        SDL_RenderCopy(renderer, alien1->texture, &source1, &destination1);
        renderText("=10 POINTS", 9, 23 * 8);
        break;
    case SCENE_PLAY:
        renderDebugText("PLAY", sceneDuration);
        // Switch to GAME scene after 1 second
        if (sceneDuration > 1000)
        {
            setScene(SCENE_GAME);
        }
        else
        {
            char buffer[256];
            sprintf(buffer, "PLAY PLAYER<%d>", player);
            renderText(buffer, 7, 14 * 8);
        }
        break;
    case SCENE_GAME:
        renderDebugText("GAME", sceneDuration);
        // // TEST : activate saucer all the time
        // if (!flagSaucer)
        // {
        //     flagSaucer = SDL_TRUE;
        //     saucer->rect.x = GAME_WIDTH + 1;
        // }
        if (flagSaucer)
        {
            saucer->rect.x -= 1;
            if (saucer->rect.x <= -16)
            {
                flagSaucer = SDL_FALSE;
            }
        }
        // Check if ship can/has to move
        if (shipDx != 0 && ship->rect.x + shipDx >= 8 && ship->rect.x + SHIP_WIDTH + shipDx <= GAME_WIDTH - 8)
        {
            ship->rect.x += shipDx;
        }
        // Check if ship touched by bombs
        // if (flagBomb)
        // {
        //     // TODO
        // }
        // Check if shoot can go up
        if (flagShoot)
        {
            if (shootExploding != 0L && ticks > shootExploding)
            {
                flagShoot = SDL_FALSE;
                shootExploding = 0L;
            }
            else
            {
                shoot->rect.y -= 1;
                if (shoot->rect.y < 4 * 8)
                {
                    shootExploding = ticks + 250;
                    shootExplosion->rect.x = shoot->rect.x - 4;
                }
            }
        }
        // Check if saucer touched by shoot
        if (flagSaucer &&
            flagShoot &&
            shoot->rect.y <= SAUCER_Y + SAUCER_HEIGHT &&
            shoot->rect.x >= saucer->rect.x &&
            shoot->rect.x <= saucer->rect.x + SAUCER_WIDTH)
        {
            saucerExploding = SDL_GetTicks() + 250;
            scores[player] += 50; // TODO
        }
        // Check if alien touched by shoot
        if (flagShoot)
        {
            // TODO
        }
        // Move aliens
        moveAliens();
        break;
    default:
        renderDebugText("????", sceneDuration);
        break;
    }

    // Render conditional elements
    renderShip();
    renderShoot();
    renderAliens();
    renderSaucer();
    renderShields();
    renderLine();
}

// EOF
