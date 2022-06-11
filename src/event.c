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

SDL_bool manageEvent(void)
{
    SDL_Event event;
    SDL_bool stop = SDL_FALSE;

    if (SDL_PollEvent(&event) == 0)
        return SDL_FALSE;
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
            if (shipVisible)
                shipDx = -1;
            break;
        case SDLK_RIGHT:
            if (shipVisible)
                shipDx = 1;
            break;
        case SDLK_SPACE:
            if (shipVisible && !shootVisible)
            {
                shootVisible = SDL_TRUE;
                shoot->rect.x = ship->rect.x + SHIP_WIDTH / 2;
                shoot->rect.y = ship->rect.y - shoot->rect.h;
                shootExploding = 0L;
            }
            break;
        case SDLK_1:
            if (scene == SCENE_HOME && credits > 0)
            {
                setScene(SCENE_PLAY);
                credits -= 1;
            }
            break;
        case SDLK_c:
            if (credits < 99)
            {
                credits += 1;
            }
            break;
        case SDLK_r:
            setScene(SCENE_BOOT);
            break;
        case SDLK_KP_ENTER:
            if (screenshotVisible)
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
            gridVisible = gridVisible ? SDL_FALSE : SDL_TRUE;
            break;
        case SDLK_d:
            debugVisible = debugVisible ? SDL_FALSE : SDL_TRUE;
            break;
        case SDLK_s:
            screenshotVisible = screenshotVisible ? SDL_FALSE : SDL_TRUE;
            stopGraphics(STATUS_RESET, NULL, NULL);
            initGraphics();
            setScene(SCENE_BOOT);
            break;
        default:
            break;
        }
        break;
    case SDL_KEYUP:
        switch (event.key.keysym.sym)
        {
        case SDLK_LEFT:
            if (shipVisible && shipDx < 0)
                shipDx = 0;
            break;
        case SDLK_RIGHT:
            if (shipVisible && shipDx > 0)
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
