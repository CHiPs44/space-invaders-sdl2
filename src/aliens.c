#include "../include/aliens.h"
#include "../include/graphics.h"
#include "../include/player.h"
#include "../include/scene.h"
#include "../include/sprite.h"

Sprite *alien1 = NULL;
Sprite *alien2 = NULL;
Sprite *alien3 = NULL;
Sprite *alienShots[3] = {NULL, NULL, NULL};
Alien aliens[ALIEN_LINES][ALIEN_COLUMNS];
int alienDelay;
int alienDown;
int alienAccel;
int alienNextMove = 0L;
int alienDx = 0;
int alienDy = 0;
SDL_bool aliensVisible = SDL_FALSE;

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
    for (uint8_t line = 0; line < ALIEN_LINES; line += 1)
    {
        int y = ALIEN_START_Y + ALIEN_GRID_Y * line;
        for (uint8_t column = 0; column < ALIEN_COLUMNS; column += 1)
        {
            int x = ALIEN_START_X + ALIEN_GRID_X * column;
            SDL_Texture *texture;
            int type;
            int frames;
            // TOP LINE: "HARD" ALIENS
            if (line == 0)
            {
                type = 3;
                texture = alien3->texture;
                frames = alien3->frames;
            }
            // LINES 1 & 2: "NOT TOO EASY" ALIENS
            else if (line <= 2)
            {
                type = 2;
                texture = alien2->texture;
                frames = alien2->frames;
            }
            // LINES 3 TO END: "EASY" ALIENS
            else
            {
                type = 1;
                texture = alien1->texture;
                frames = alien1->frames;
            }
            Alien alien;
            alien.sprite = createSpriteFromTexture(texture, frames);
            alien.type = type;
            alien.state = ALIEN_IS_ALIVE;
            alien.sprite->rect.x = x;
            alien.sprite->rect.y = y;
            alien.explosionTicks = 0L;
            aliens[line][column] = alien;
        }
    }
    alienDelay = 512L;
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
    int ticks = SDL_GetTicks();
    if (alienNextMove == 0L)
    {
        // Start move
        alienNextMove = ticks + alienDelay;
    }
    else if (ticks < alienNextMove)
    {
        // Wait until next move
        return;
    }
    // Set next move
    alienNextMove = ticks + alienDelay;
    int minX = GAME_WIDTH;
    int maxX = 0;
    int maxY = 0;
    int aliveCount = 0;
    for (uint8_t line = 0; line < ALIEN_LINES; line += 1)
    {
        for (uint8_t column = 0; column < ALIEN_COLUMNS; column += 1)
        {
            Alien *alien = &aliens[line][column];
            if (alien->state == ALIEN_IS_ALIVE)
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
            else if (alien->state == ALIEN_IS_EXPLODING)
            {
                alien->sprite->frame = 2;
            }
        }
    }
    // Reached left or right border?
    alienDy = 0;
    if ((alienDx == -1 && minX < 8) || (alienDx == 1 && maxX > GAME_WIDTH - 8))
    {
        alienDx = -alienDx;
        alienDy = 1;
        alienDown += 1;
        if (alienDown > 8)
        {
            alienDelay -= alienAccel;
            if (alienDelay < 0)
                alienDelay = 0;
            alienDown = 0;
        }
    }
}

void renderAliens(uint32_t ticks)
{
    if (!aliensVisible)
        return;
    for (uint8_t line = 0; line < ALIEN_LINES; line += 1)
    {
        for (uint8_t column = 0; column < ALIEN_COLUMNS; column += 1)
        {
            Alien *alien = &aliens[line][column];
            if (alien->state == ALIEN_IS_EXPLODING)
            {
                if (ticks > alien->explosionTicks)
                {
                    alien->state = ALIEN_IS_DEAD;
                    alien->explosionTicks = 0L;
                } else {
                    
                }
            }
            if (alien->state != ALIEN_IS_DEAD)
            {
                renderSprite(alien->sprite);
            }
        }
    }
}

SDL_bool shootAtAliens(uint32_t ticks)
{
    for (uint8_t line = 0; line < ALIEN_LINES; line += 1)
    {
        for (uint8_t column = 0; column < ALIEN_COLUMNS; column += 1)
        {
            Alien *alien = &aliens[line][column];
            if (alien->state != ALIEN_IS_DEAD)
            {
                if (isIntersectingSprite(shoot, alien->sprite))
                {
                    alien->state = ALIEN_IS_EXPLODING;
                    alien->explosionTicks = ticks + 500L;
                    return SDL_TRUE;
                }
            }
        }
    }
    return SDL_FALSE;
}

// EOF
