// gcc main.c $(sdl2-config --cflags --libs) -lSDL2_image
// gcc -std=c99 -o main -Wall -Wextra -pedantic main.c -lSDL2 -lSDL2_image
// ./main

// https://zestedesavoir.com/tutoriels/1014/utiliser-la-sdl-en-langage-c/

// https://blog.loadzero.com/blog/si78c/

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../include/const.h"
#include "../include/graphics.h"
#include "../include/font.h"
#include "../include/debug.h"
#include "../include/player.h"
#include "../include/sprite.h"
#include "../include/scene.h"

// #define WITH_SCREENSHOTS SDL_TRUE

/**
 * @brief Here we go!
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[])
{
    SDL_Event event;

    graphicsInit();

    // Load assets
    initFont();
    // initPlayer();
    // initAliens();
    // initSaucer();
    // initDebug();

    // setScene(SCENE_BOOT);

    SDL_bool stop = SDL_FALSE;
    while (!stop)
    {
        // uint32_t frameBegin = SDL_GetTicks();
        // // Draw current scene
        // renderScene();
        // // Draw screenshot
        // // Helps debugging item positions
        // renderGrid();
        // // Render complete screen
        // SDL_RenderPresent(renderer);
        // Manage events
        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            stop = 1;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                stop = 1;
                break;
            // case SDLK_LEFT:
            //     if (flagShip)
            //         shipDx = -1;
            //     break;
            // case SDLK_RIGHT:
            //     if (flagShip)
            //         shipDx = 1;
            //     break;
            // case SDLK_SPACE:
            //     if (flagShip && !flagShoot)
            //     {
            //         flagShoot = SDL_TRUE;
            //         shoot->rect.x = ship->rect.x + SHIP_WIDTH / 2;
            //         shoot->rect.y = ship->rect.y - shoot->rect.w;
            //     }
            // case SDLK_1:
            //     if (scene == SCENE_HOME && credits > 0)
            //     {
            //         setScene(SCENE_PLAY);
            //         credits -= 1;
            //     }
            //     break;
            // case SDLK_r:
            //     setScene(SCENE_BOOT);
            //     break;
            // case SDLK_KP_ENTER:
            //     screenshot = (screenshot + 1) % 3;
            //     break;
            // case SDLK_KP_PLUS:
            //     speed += 1;
            //     if (speed > 99)
            //     {
            //         speed = 99;
            //     }
            //     break;
            // case SDLK_KP_MINUS:
            //     speed -= 1;
            //     if (speed < 0)
            //     {
            //         speed = 0;
            //     }
            //     break;
            // case SDLK_g:
            //     grid = 1 - grid;
            //     break;
            default:
                break;
            }
            break;
        // case SDL_KEYUP:
        //     switch (event.key.keysym.sym)
        //     {
        //     case SDLK_LEFT:
        //         if (flagShip && shipDx < 0)
        //             shipDx = 0;
        //         break;
        //     case SDLK_RIGHT:
        //         if (flagShip && shipDx > 0)
        //             shipDx = 0;
        //         break;
        //     default:
        //         break;
        //     }
        //     break;
        default:
            break;
        }
        // uint32_t renderEnd = SDL_GetTicks();
        // int32_t renderTicks = renderEnd - frameBegin;
        // delay = speed - renderTicks;
        // if (delay < 0)
        //     delay = 0;
        // if (delay > 100)
        //     delay = 100;
        // SDL_Delay(delay);
        // uint32_t frameEnd = SDL_GetTicks();
        // uint32_t frameDuration = frameEnd - frameBegin;
    }

    // Fin des haricots
    graphicsQuit(EXIT_SUCCESS, NULL);
}

// EOF
