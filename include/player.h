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
extern uint8_t player; // current player
extern int shipDx; // -1..1
extern int playerExploding; // end of exploding player
extern int shootExploding;

void initPlayer();
void renderShip();
void renderShoot();

#endif

// EOF
