#include "sprite.h"

#ifndef PLAYER_H

#define PLAYER_H

#define SHIP_WIDTH 13
#define SHIP_HEIGHT 8
#define SHIP_Y 216

extern Sprite *ship;
extern Sprite *shoot;
extern Sprite *shootExplosion;
extern SDL_bool flagShip;
extern SDL_bool flagShoot;
extern uint8_t player; // current player 1..2
extern int8_t shipDx; // -1..1
extern uint32_t playerExploding; // end of exploding player
extern uint32_t shootExploding;

void initPlayer(void);
void renderShip(void);
void renderShoot(void);

#endif

// EOF
