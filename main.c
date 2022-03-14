// gcc main.c $(sdl2-config --cflags --libs) -lSDL2_image
// gcc -std=c99 -o main -Wall -Wextra -pedantic main.c -lSDL2 -lSDL2_image
// ./main

// https://zestedesavoir.com/tutoriels/1014/utiliser-la-sdl-en-langage-c/

// https://blog.loadzero.com/blog/si78c/

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>

#define WIDTH 224
#define HEIGHT 256
#define ZOOM 3

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *background = NULL;
SDL_Texture *ship = NULL;
SDL_Texture *digits = NULL;

void quit(int status, char *message)
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

void init()
{
    char title[256];
    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        quit(EXIT_FAILURE, "SDL_Init");
    }
    // Notre fenêtre pour jouer
    sprintf(title, "Space Invaders %dx%dx%d", WIDTH, HEIGHT, ZOOM);
    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH * ZOOM, HEIGHT * ZOOM,
        SDL_WINDOW_SHOWN);
    if (NULL == window)
    {
        quit(EXIT_FAILURE, "SDL_CreateWindow");
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (NULL == renderer)
    {
        quit(EXIT_FAILURE, "SDL_CreateRenderer");
    }
    IMG_Init(IMG_INIT_PNG);
}

void renderDigit(uint8_t digit, uint8_t x, uint8_t y)
{
    SDL_Rect src = {8 * digit, 0, 8, 8};
    SDL_Rect dst = {x * ZOOM, y * ZOOM, 8 * ZOOM, 8 * ZOOM};
    SDL_RenderCopy(renderer, digits, &src, &dst);
}

void renderScore(uint16_t score, uint8_t player)
{
    int x = player == 1 ? 16 : 160;
    score = score % 10000;
    for (uint8_t n = 4; n > 0; n -= 1)
    {
        uint8_t digit = score % 10;
        renderDigit(digit, x + 8 * n, 25);
        score = score / 10;
    }
}

void renderShip(uint8_t x)
{
    SDL_Rect shipRect = {x * ZOOM, 216 * ZOOM, 13 * ZOOM, 8 * ZOOM};
    SDL_RenderCopy(renderer, ship, NULL, &shipRect);
}
void renderShipsLeft(uint8_t left)
{
    // SDL_Rect shipRect = {x * ZOOM, 216 * ZOOM, 13 * ZOOM, 8 * ZOOM};
    // SDL_RenderCopy(renderer, ship, NULL, &shipRect);
}

int main(int argc, char *argv[])
{
    SDL_Event event;

    // Au début, il n'y avait rien
    init();

    // Charger des images
    background = IMG_LoadTexture(renderer, "./background.png");
    ship = IMG_LoadTexture(renderer, "./ship.png");
    digits = IMG_LoadTexture(renderer, "./digits.png");
    fprintf(stderr, "Error %s", IMG_GetError());

    // Faire des choses !
    int player = 1;
    int scores[2] = {850, 0};
    int shipX = (WIDTH - 13) / 2;
    int dx = 0;
    int stop = 0;
    while (!stop)
    {
        SDL_RenderCopy(renderer, background, NULL, NULL);
        renderScore(scores[0], 1);
        renderScore(scores[1], 2);
        renderShip(shipX);
        SDL_RenderPresent(renderer);
        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            stop = 1;
            break;
        /* Look for a keypress */
        case SDL_KEYDOWN:
            /* Check the SDLKey values and move change the coords */
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                dx = 0;
                stop = 1;
                break;
            case SDLK_LEFT:
                dx = -1;
                break;
            case SDLK_RIGHT:
                dx = 1;
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_LEFT:
                if (dx < 0)
                    dx = 0;
                break;
            case SDLK_RIGHT:
                if (dx > 0)
                    dx = 0;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        if (shipX + dx >= 0 && shipX + 13 + dx <= WIDTH)
        {
            shipX += dx;
        }
        SDL_Delay(10);
    }

    // Fin des haricots
    quit(EXIT_SUCCESS, NULL);
}
