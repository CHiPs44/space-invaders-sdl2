#include <SDL2/SDL.h>
#include "sprite.h"

#ifndef ALIENS_H

#define ALIENS_H

#define ALIEN_WIDTH 16
#define ALIEN_HEIGHT 8

#define ALIEN_LINES 5
#define ALIEN_COLUMNS 11

#define ALIEN_START_X (2 * 8)
#define ALIEN_START_Y (8 * 8)

#define ALIEN_GRID_X 16
#define ALIEN_GRID_Y 16

#define ALIEN1_WIDTH 16
#define ALIEN1_HEIGHT 8

#define ALIEN2_WIDTH 16
#define ALIEN2_HEIGHT 8

#define ALIEN3_WIDTH 16
#define ALIEN3_HEIGHT 8

extern Sprite *alien1;
extern Sprite *alien2;
extern Sprite *alien3;
extern SDL_bool flagAliens;

typedef struct
{
    Sprite *sprite;
    int type; // 1..3
    int exploding;
    int bombing;
    SDL_bool dead;
} Alien;

extern Alien aliens[ALIEN_LINES][ALIEN_COLUMNS];
extern SDL_bool flagAliens;

void initAliens(void);
void resetAliens(void);
void moveAliens(void);
void renderAliens(void);

#endif

// EOF
