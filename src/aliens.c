#include "../include/aliens.h"
#include "../include/const.h"
#include "../include/graphics.h"
#include "../include/scene.h"

Sprite *alien1 = NULL;
Sprite *alien2 = NULL;
Sprite *alien3 = NULL;
Sprite *alienShots[] = {NULL, NULL, NULL};
Alien aliens[ALIEN_LINES][ALIEN_COLUMNS];
int alienSpeed = 500L;
int alienLastMove = 0L;
int alienDx = 0;
int alienDy = 0;
SDL_bool aliensFlag = SDL_FALSE;

/**
 * @brief Initialize aliens: sprites and locations
 */
void initAliens(void)
{
    alien1 = createSpriteFromFile("alien1.png", 2);
    alien2 = createSpriteFromFile("alien2.png", 2);
    alien3 = createSpriteFromFile("alien3.png", 2);
    alienShots[ALIEN_ROLLING_SHOT] = createSpriteFromFile("rolling.png", 2);
    alienShots[ALIEN_PLUNGER_SHOT] = createSpriteFromFile("plunger.png", 2);
    alienShots[ALIEN_SQUIGGLY_SHOT] = createSpriteFromFile("squiggly.png", 2);
    resetAliens();
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
            alien.dead = SDL_FALSE;
            alien.sprite->rect.x = x;
            alien.sprite->rect.y = y;
            aliens[line][column] = alien;
        }
    }
    alienLastMove = 0L;
    alienDx = 1;
    alienDy = 0;
}

void moveAliens(void)
{
    int now = SDL_GetTicks();
    if (alienLastMove == 0L)
    {
        alienLastMove = now;
    }
    else if (alienLastMove + alienSpeed < now)
    {
        return; // Wait until next move
    }
    else
    {
        alienLastMove = now;
    }
    fprintf(stderr, "moveAliens: %d, dx=%d, dy=%d\n", alienLastMove, alienDx, alienDy);
    // TODO check reached right?
    // TODO check reached left?
    for (int line = 0; line < ALIEN_LINES; line += 1)
    {
        for (int column = 0; column < ALIEN_COLUMNS; column += 1)
        {
            Alien *alien = &aliens[line][column];
            if (!alien->dead)
            {
                // 1-0=>1, 0-1=>0
                alien->sprite->frame = 1 - alien->sprite->frame;
                alien->sprite->rect.x += alienDx;
                alien->sprite->rect.y += alienDy;
            }
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
            if (!alien->dead)
            {
                renderSprite(alien->sprite);
            }
        }
    }
}

// EOF
