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
#define SCENE_OVER 5

extern int32_t speed;
extern int32_t delay;
extern uint32_t fps;
extern uint8_t scene;
extern char sceneName[16];
extern uint32_t sceneStart;
extern uint32_t sceneDuration;
extern uint32_t step;

extern uint32_t frameDuration;
extern uint32_t frameStart;
extern uint32_t frames;
extern SDL_bool frameChanged;

extern uint8_t playMode; // 1=1 player, 2=2 players

void initScene(void);
void setScene(uint8_t newScene);
void renderScene(void);
void renderSceneBoot(void);
void renderSceneHome(void);
void renderScenePlay(void);
void renderSceneGame(uint32_t ticks);

#endif

// EOF
