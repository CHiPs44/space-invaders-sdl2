#include "../include/graphics.h"
#include "../include/const.h"

int zoom = 3;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void graphicsQuit(int status, char *message)
{
    IMG_Quit();
    if (NULL != message)
    {
        fprintf(stderr, "%s : %s", message, SDL_GetError());
    }
    if (NULL != renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (NULL != window)
    {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
    exit(status);
}

void graphicsInit()
{
    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        graphicsQuit(EXIT_FAILURE, "SDL_Init");
    }
    char title[64];
    sprintf(title, "Space Invaders 1978 (%dx%dx%d)", GAME_WIDTH, GAME_HEIGHT, zoom);
    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        GAME_WIDTH * zoom * 2, (GAME_HEIGHT + 16) * zoom,
        SDL_WINDOW_SHOWN);
    if (NULL == window)
    {
        graphicsQuit(EXIT_FAILURE, "SDL_CreateWindow");
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // | SDL_RENDERER_PRESENTVSYNC);
    if (NULL == renderer)
    {
        graphicsQuit(EXIT_FAILURE, "SDL_CreateRenderer");
    }
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        graphicsQuit(EXIT_FAILURE, "IMG_Init");
    }
}

// EOF
