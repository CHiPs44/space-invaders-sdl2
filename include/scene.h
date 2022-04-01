/*
 * Scene management
 */

#include <SDL2/SDL.h>

#ifndef SCENE_H

#define SCENE_H

#define SCENE_NONE 0
#define SCENE_BOOT 1
#define SCENE_HOME 2
#define SCENE_PLAY 3
#define SCENE_GAME 4

extern int32_t speed;
extern int32_t delay;

extern uint8_t scene;
extern uint32_t sceneStart;
extern uint32_t sceneDuration;
extern uint32_t step;

extern uint8_t playMode; // 1=1 player, 2=2 players

void setScene(uint8_t newScene);
void renderScene(void);

#endif

// EOF
