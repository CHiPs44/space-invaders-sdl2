#ifndef ITEMS_H

#define ITEMS_H

extern uint16_t scores[3]; // 0=HI, 1=P1, 2=P2
extern uint8_t lives; // 0..4
extern uint8_t credits; // 0..99
extern SDL_bool lineFlag;
extern SDL_bool livesFlag;

extern void renderScores(void);
extern void renderLives(void);
extern void renderCredits(void);
extern void renderLine(void);

#endif

// EOF
