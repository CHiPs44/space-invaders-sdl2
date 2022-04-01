#include "sprite.h"

#ifndef SAUCER_H

#define SAUCER_H

#define SAUCER_WIDTH 16
#define SAUCER_HEIGHT 8
#define SAUCER_Y 40

extern Sprite *saucer;
extern SDL_bool flagSaucer;
extern Uint32 saucerExploding; // end of exploding saucer

void initSaucer(void);
// void quitSaucer(void);
void renderSaucer(void);

#endif

// EOF
