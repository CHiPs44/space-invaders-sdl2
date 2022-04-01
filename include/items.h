#ifndef ITEMS_H

#define ITEMS_H

extern uint16_t scores[3]; // 0=HI, 1=P1, 2=P2
extern uint8_t lives; // 0..4
extern uint8_t credits; // 0..99
extern SDL_bool flagLine;
extern SDL_bool flagLives;

extern void renderScores();
extern void renderLives();
extern void renderCredits();
extern void renderLine();

#endif

// EOF
