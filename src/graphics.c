#include "../include/graphics.h"
#include "../include/debug.h"

int zoom = 2;
int widthFactor;
int windowWidth;
int windowHeight;
int offsetX;
int offsetY;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

/**
 * @brief Reset SDL, optionnally print error message then quit
 *
 * @param status  EXIT_SUCCESS, EXIT_FAILURE, STATUS_RESET
 * @param message generally function name of failure
 * @param error   generally SDL_GetError()
 */
void stopGraphics(int status, const char *message, const char *error)
{
    IMG_Quit();
    if (NULL != message && NULL != error)
    {
        fprintf(stderr, "%s: %s\n", message, error);
    }
    if (NULL != message)
    {
        fprintf(stderr, "%s\n", message);
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
    if (STATUS_RESET != status)
    {
        exit(status);
    }
}

void initGraphics(void)
{
    if (0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        stopGraphics(EXIT_FAILURE, "SDL_Init", SDL_GetError());
    }
    if (0 == IMG_Init(IMG_INIT_PNG))
    {
        stopGraphics(EXIT_FAILURE, "IMG_Init", SDL_GetError());
    }
    resizeGraphics();
}

void resizeGraphics(void)
{
    widthFactor = screenshotVisible ? 2 : 1;
    windowWidth = WINDOW_WIDTH * zoom * widthFactor;
    windowHeight = WINDOW_HEIGHT * zoom;
    offsetX = (windowWidth - GAME_WIDTH * widthFactor * zoom) / 2;
    offsetY = (windowHeight - GAME_HEIGHT * zoom) / 2;
    if (NULL == window)
    {
        char title[256];
        snprintf(
            title, 256,
            "Space Invaders 1978 (%dx%dx%d=>%d+%dx%d+%d)",
            GAME_WIDTH, GAME_HEIGHT, zoom,
            windowWidth, offsetX, windowHeight, offsetY);
        window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            windowWidth, windowHeight,
            SDL_WINDOW_SHOWN);
        if (NULL == window)
        {
            stopGraphics(EXIT_FAILURE, "SDL_CreateWindow", SDL_GetError());
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // | SDL_RENDERER_PRESENTVSYNC);
        if (NULL == renderer)
        {
            stopGraphics(EXIT_FAILURE, "SDL_CreateRenderer", SDL_GetError());
        }
    }
    else
    {
        SDL_SetWindowSize(window, windowWidth, windowHeight);
        SDL_SetWindowPosition(
            window,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED);
    }
}

// EOF
