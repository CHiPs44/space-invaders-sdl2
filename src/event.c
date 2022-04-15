#include "../include/event.h"

#include "../include/aliens.h"
#include "../include/debug.h"
#include "../include/font.h"
#include "../include/graphics.h"
#include "../include/items.h"
#include "../include/player.h"
#include "../include/saucer.h"
#include "../include/scene.h"
#include "../include/sprite.h"

SDL_Event event;

SDL_bool manageEvent(void)
{
    SDL_bool stop = SDL_FALSE;

    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        stop = SDL_TRUE;
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            stop = SDL_TRUE;
            break;
        case SDLK_LEFT:
            if (shipFlag)
                shipDx = -1;
            break;
        case SDLK_RIGHT:
            if (shipFlag)
                shipDx = 1;
            break;
        case SDLK_SPACE:
            if (shipFlag && !shootFlag)
            {
                shootFlag = SDL_TRUE;
                shoot->rect.x = ship->rect.x + SHIP_WIDTH / 2;
                shoot->rect.y = ship->rect.y - shoot->rect.w;
            }
            break;
        case SDLK_1:
            if (scene == SCENE_HOME && credits > 0)
            {
                setScene(SCENE_PLAY);
                credits -= 1;
            }
            break;
        case SDLK_r:
            setScene(SCENE_BOOT);
            break;
        case SDLK_KP_ENTER:
            screenshot = (screenshot + 1) % 3;
            break;
        case SDLK_KP_PLUS:
            speed += 1;
            if (speed > 50)
            {
                speed = 50;
            }
            break;
        case SDLK_KP_MINUS:
            speed -= 1;
            if (speed < 0)
            {
                speed = 0;
            }
            break;
        case SDLK_g:
            grid = 1 - grid;
            break;
        default:
            break;
        }
        break;
    case SDL_KEYUP:
        switch (event.key.keysym.sym)
        {
        case SDLK_LEFT:
            if (shipFlag && shipDx < 0)
                shipDx = 0;
            break;
        case SDLK_RIGHT:
            if (shipFlag && shipDx > 0)
                shipDx = 0;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return stop;
}

// EOF
