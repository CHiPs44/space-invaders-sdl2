#include <SDL2/SDL.h>

#ifndef SHIELD_H

#define SHIELD_H

#define SHIELD_WIDTH 24
#define SHIELD_HEIGHT 16
#define SHIELD_Y 192

extern SDL_bool shieldsVisible;

void initShields(void);
void renderShields(void);
void shootAtShield(int bombType, uint8_t x, uint8_t y);

#endif

// EOF
