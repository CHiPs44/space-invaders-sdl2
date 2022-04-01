#include "sprite.h"

#ifndef SAUCER_H

#define SAUCER_H

#define SAUCER_WIDTH 16
#define SAUCER_HEIGHT 8
#define SAUCER_Y 40

extern Sprite *saucer;
extern int saucerExploding; // end of exploding saucer
extern SDL_bool flagSaucer;

void initSaucer();
// void quitSaucer();
void renderSaucer();

#endif

// EOF
