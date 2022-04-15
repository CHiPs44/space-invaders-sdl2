// gcc -std=c99 -o main -Wall -Wextra -pedantic main.c -lSDL2 -lSDL2_image
// ./main

// https://zestedesavoir.com/tutoriels/1014/utiliser-la-sdl-en-langage-c/

// https://blog.loadzero.com/blog/si78c/
// https://computerarcheology.com/Arcade/SpaceInvaders/

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "../include/aliens.h"
#include "../include/debug.h"
#include "../include/event.h"
#include "../include/font.h"
#include "../include/graphics.h"
#include "../include/items.h"
#include "../include/player.h"
#include "../include/saucer.h"
#include "../include/scene.h"
#include "../include/shields.h"
#include "../include/sprite.h"

/**
 * @brief Here we go!
 */
int main(void /*int argc, char *argv[]*/)
{
    initGraphics();

    // Load assets
    initAliens();
    initDebug();
    initFont();
    initPlayer();
    initSaucer();
    initShields();

    initScene();
    setScene(SCENE_BOOT);

    while (SDL_TRUE)
    {
        uint32_t renderBegin = SDL_GetTicks();
        // Draw current scene
        renderScene();
        // Draw debug & screenshot
        renderDebugText();
        renderScreenshot();
        // Helps debugging item positions
        renderGrid();
        // Render complete screen
        SDL_RenderPresent(renderer);
        // Manage event
        if (manageEvent())
            break;
        uint32_t renderEnd = SDL_GetTicks();
        int32_t renderTicks = renderEnd - renderBegin;
        delay = speed - renderTicks;
        if (delay < 0)
            delay = 0;
        if (delay > 100)
            delay = 100;
        SDL_Delay(delay);
        // uint32_t frameEnd = SDL_GetTicks();
        // uint32_t frameDuration = frameEnd - frameBegin;
    }

    // Fin des haricots
    stopGraphics(EXIT_SUCCESS, NULL, NULL);
}

// EOF
