/*
 * Scene management
 */

#include "../include/scene.h"
#include "../include/debug.h"
#include "../include/font.h"
#include "../include/graphics.h"
#include "../include/items.h"
#include "../include/player.h"
#include "../include/aliens.h"
#include "../include/shields.h"
#include "../include/saucer.h"

int32_t speed = 7;
int32_t delay;

uint32_t fps = 25;

uint8_t scene = SCENE_NONE;
char sceneName[16] = "";
uint32_t sceneStart;
uint32_t sceneDuration;
uint32_t frameDuration;
uint32_t frameStart;
uint32_t frames;
SDL_bool frameChanged;
uint8_t playMode; // 1=1 player, 2=2 players

void initScene(void)
{
    // scene = SCENE_NONE;
    strcpy(sceneName, "");
    sceneStart = 0L;
    sceneDuration = 0L;
    frameDuration = 0L;
    frameStart = 0L;
    frames = 0L;
    frameChanged = SDL_FALSE;
    playMode = 0;
}

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
        // Display "garbage" on screen 0
        strcpy(sceneName, "Boot");
        break;
    case SCENE_HOME:
        // Display PLAY SPACE INVADERS, "instructions" & aliens points
        strcpy(sceneName, "Home");
        break;
    case SCENE_PLAY:
        // Start 1 player game (for now)
        strcpy(sceneName, "Play");
        playMode = 1;
        player = 1;
        break;
    case SCENE_GAME:
        // Effectively play game
        strcpy(sceneName, "Game");
        ship->rect.x = (GAME_WIDTH - SHIP_WIDTH) / 2;
        shipDx = 0;
        flagLine = SDL_TRUE;
        flagLives = SDL_TRUE;
        flagShip = SDL_TRUE;
        flagAliens = SDL_TRUE;
        flagShields = SDL_TRUE;
        break;
    default:
        strcpy(sceneName, "????");
        fprintf(stderr, "Unknown scene #%d\n", newScene);
        break;
    }
    scene = newScene;
    uint32_t ticks = SDL_GetTicks();
    sceneStart = ticks;
    frameStart = 0L;
    frames = 0;
}

void renderScene(void)
{
    uint32_t ticks = SDL_GetTicks();
    sceneDuration = ticks - sceneStart;
    frameDuration = ticks - frameStart;
    frameChanged = SDL_FALSE;
    if (frameDuration > 1000 / fps)
    {
        frameStart = ticks;
        frames += 1;
        frameChanged = SDL_TRUE;
    }
    else
    {
        frameChanged = SDL_FALSE;
    }
    // if (!frameChanged)
    //     return;

    // Clear screen to black
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
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
        renderSceneBoot();
        break;
    case SCENE_HOME:
        renderSceneHome();
        break;
    case SCENE_PLAY:
        renderScenePlay();
        break;
    case SCENE_GAME:
        renderSceneGame(ticks);
        break;
    default:
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

void renderSceneBoot(void)
{
    // Switch to HOME scene after 1 second
    if (sceneDuration > 1000)
    {
        setScene(SCENE_HOME);
    }
    else
    {
        // Display random chars
        for (uint8_t line = 0; line < GAME_HEIGHT / 8; line++)
        {
            for (uint8_t column = 0; column < GAME_WIDTH / 8; column++)
            {
                // Progressively display less chars
                int c = rand() % (2 * sceneDuration);
                if (c <= 255)
                {
                    // 50% of DEL (block)
                    renderChar(c > 127 ? 127 : c, column, line * 8);
                }
            }
        }
    }
}

void renderSceneHome(void)
{
    // Title
    renderText("PLAY", 12, 8 * 8);
    renderText("SPACE  INVADERS", 7, 11 * 8);
    renderText("*SCORE ADVANCE TABLE*", 3, 15 * 8);
    // Saucer
    SDL_Rect sourceS = {0, 0, SAUCER_WIDTH, SAUCER_HEIGHT};
    SDL_Rect destinationS =
        {(9 - 2) * 8 * zoom, 17 * 8 * zoom,
         SAUCER_WIDTH * zoom, SAUCER_HEIGHT * zoom};
    SDL_RenderCopy(renderer, saucer->texture, &sourceS, &destinationS);
    renderText("=? MYSTERY", 9, 17 * 8);
    // Alien type 3
    SDL_Rect source3 =
        {0, 0,
         ALIEN3_WIDTH, ALIEN3_HEIGHT};
    SDL_Rect destination3 =
        {(9 - 2) * 8 * zoom, 19 * 8 * zoom,
         ALIEN3_WIDTH * zoom, ALIEN3_HEIGHT * zoom};
    SDL_RenderCopy(renderer, alien3->texture, &source3, &destination3);
    renderText("=30 POINTS", 9, 19 * 8);
    // Alien type 2
    SDL_Rect source2 =
        {0, 0,
         ALIEN2_WIDTH, ALIEN2_HEIGHT};
    SDL_Rect destination2 =
        {(9 - 2) * 8 * zoom, 21 * 8 * zoom,
         ALIEN2_WIDTH * zoom, ALIEN2_HEIGHT * zoom};
    SDL_RenderCopy(renderer, alien2->texture, &source2, &destination2);
    renderText("=20 POINTS", 9, 21 * 8);
    // Alien type 1
    SDL_Rect source1 =
        {0, 0,
         ALIEN1_WIDTH, ALIEN1_HEIGHT};
    SDL_Rect destination1 =
        {(9 - 2) * 8 * zoom, 23 * 8 * zoom,
         ALIEN1_WIDTH * zoom, ALIEN1_HEIGHT * zoom};
    SDL_RenderCopy(renderer, alien1->texture, &source1, &destination1);
    renderText("=10 POINTS", 9, 23 * 8);
}

void renderScenePlay(void)
{
    // Switch to GAME scene after 1 second
    if (sceneDuration > 1000)
    {
        setScene(SCENE_GAME);
    }
    else
    {
        char buffer[20];
        sprintf(buffer, "PLAY PLAYER<%d>", player);
        renderText(buffer, 7, 14 * 8);
    }
}

void renderSceneGame(uint32_t ticks)
{
    // // TEST : activate saucer all the time
    // if (!flagSaucer)
    // {
    //     flagSaucer = SDL_TRUE;
    //     saucer->rect.x = GAME_WIDTH + 1;
    // }
    if (frameChanged && flagSaucer)
    {
        saucer->rect.x -= 1;
        if (saucer->rect.x <= -16)
        {
            flagSaucer = SDL_FALSE;
        }
    }
    // Check if ship can/has to move
    if (frameChanged &&
        shipDx != 0 &&
        ship->rect.x + shipDx >= 8 &&
        ship->rect.x + SHIP_WIDTH + shipDx <= GAME_WIDTH - 8)
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
}

// EOF
