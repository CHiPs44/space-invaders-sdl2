#include "../include/aliens.h"
#include "../include/const.h"
#include "../include/graphics.h"
#include "../include/scene.h"

Sprite *alien1 = NULL;
Sprite *alien2 = NULL;
Sprite *alien3 = NULL;
Sprite *alienShots[] = {NULL, NULL, NULL};
Alien aliens[ALIEN_LINES][ALIEN_COLUMNS];
int alienDelay;
int alienDown;
int alienAccel;
int alienNextMove = 0L;
int alienDx = 0;
int alienDy = 0;
SDL_bool aliensFlag = SDL_FALSE;

/**
 * @brief Initialize aliens sprites
 */
void initAliens(void)
{
    alien1 = createSpriteFromFile("alien1.png", 2);
    alien2 = createSpriteFromFile("alien2.png", 2);
    alien3 = createSpriteFromFile("alien3.png", 2);
    alienShots[ALIEN_ROLLING_SHOT] = createSpriteFromFile("rolling.png", 2);
    alienShots[ALIEN_PLUNGER_SHOT] = createSpriteFromFile("plunger.png", 2);
    alienShots[ALIEN_SQUIGGLY_SHOT] = createSpriteFromFile("squiggly.png", 2);
}

/**
 * @brief Reset aliens to start locations
 */
void resetAliens(void)
{
    for (int line = 0; line < ALIEN_LINES; line += 1)
    {
        int y = ALIEN_START_Y + ALIEN_GRID_Y * line;
        for (int column = 0; column < ALIEN_COLUMNS; column += 1)
        {
            int x = ALIEN_START_X + ALIEN_GRID_X * column;
            SDL_Texture *texture;
            int type;
            int frames;
            if (line == 0)
            {
                type = 3;
                texture = alien3->texture;
                frames = alien3->frames;
            }
            else if (line <= 2)
            {
                type = 2;
                texture = alien2->texture;
                frames = alien2->frames;
            }
            else
            {
                type = 1;
                texture = alien1->texture;
                frames = alien1->frames;
            }
            Alien alien;
            alien.sprite = createSpriteFromTexture(texture, frames);
            alien.type = type;
            alien.bombing = SDL_FALSE;
            alien.alive = SDL_TRUE;
            alien.sprite->rect.x = x;
            alien.sprite->rect.y = y;
            aliens[line][column] = alien;
        }
    }
    alienDelay = 128L; // 512L;
    alienAccel = 64L;
    alienNextMove = 0L;
    alienDx = 1;
    alienDy = 0;
    alienDown = 0;
}

/**
 * @brief Move aliens left or right and down eventually
 */
void moveAliens(void)
{
    int now = SDL_GetTicks();
    if (alienNextMove == 0L)
    {
        // Start move
        alienNextMove = now + alienDelay;
    }
    else if (now < alienNextMove)
    {
        // Wait until next move
        return;
    }
    // Set next move
    alienNextMove = now + alienDelay;
    int minX = GAME_WIDTH;
    int maxX = 0;
    int maxY = 0;
    int aliveCount = 0;
    for (int line = 0; line < ALIEN_LINES; line += 1)
    {
        for (int column = 0; column < ALIEN_COLUMNS; column += 1)
        {
            Alien *alien = &aliens[line][column];
            if (alien->alive)
            {
                aliveCount += 1;
                // Change frame: 1-0=>1, 0-1=>0
                alien->sprite->frame = 1 - alien->sprite->frame;
                alien->sprite->rect.x += alienDx;
                alien->sprite->rect.y += alienDy;
                minX = alien->sprite->rect.x < minX
                           ? alien->sprite->rect.x
                           : minX;
                maxX = alien->sprite->rect.x + ALIEN1_WIDTH > maxX
                           ? alien->sprite->rect.x + ALIEN1_WIDTH
                           : maxX;
                maxY = alien->sprite->rect.y + ALIEN1_HEIGHT > maxY
                           ? alien->sprite->rect.y + ALIEN1_HEIGHT
                           : maxY;
            }
        }
    }
    alienDy = 0;
    if ((alienDx == -1 && minX < 8) || (alienDx == 1 && maxX > GAME_WIDTH - 8))
    {
        alienDx = -alienDx;
        alienDy = 1;
        alienDown += 1;
        if (alienDown > 8)
        {
            alienDelay -= alienAccel;
            alienDown = 0;
        }
    }
}

void renderAliens(void)
{
    if (!aliensFlag)
        return;
    for (int line = 0; line < ALIEN_LINES; line += 1)
    {
        for (int column = 0; column < ALIEN_COLUMNS; column += 1)
        {
            Alien *alien = &aliens[line][column];
            if (alien->alive)
            {
                renderSprite(alien->sprite);
            }
        }
    }
}

// EOF
