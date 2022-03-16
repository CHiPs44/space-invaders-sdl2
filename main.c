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
SDL_Texture *chars = NULL;

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
    char title[64];
    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        quit(EXIT_FAILURE, "SDL_Init");
    }
    // Notre fenêtre pour jouer
    sprintf(title, "Space Invaders 1978 %dx%dx%d", WIDTH, HEIGHT, ZOOM);
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
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (NULL == renderer)
    {
        quit(EXIT_FAILURE, "SDL_CreateRenderer");
    }
    IMG_Init(IMG_INIT_PNG);
}

void renderChar(char c, uint8_t column, uint8_t y)
{
    uint8_t offset_x = (c % 32) * 8;
    uint8_t offset_y = (c / 32) * 8;
    SDL_Rect src = {offset_x, offset_y, 8, 8};
    // fprintf(stderr, "%c %d %d\r\n", c, offset_x, offset_y);
    SDL_Rect dst = {column * 8 * ZOOM, y * ZOOM, 8 * ZOOM, 8 * ZOOM};
    SDL_RenderFillRect(renderer, &dst);
    SDL_RenderCopy(renderer, chars, &src, &dst);
}

void renderText(char *s, uint8_t column, uint8_t line)
{
    uint8_t i = 0;
    while (s[i])
    {
        renderChar(s[i], column + i, line);
        i += 1;
    }
}

void renderNumber(uint16_t number, uint8_t width, uint8_t column, uint8_t y)
{
    char buffer[width + 1];
    sprintf(buffer, "%0*d", width, number);
    renderText(buffer, column, y);
}

void renderScore(uint16_t score, uint8_t player)
{
    int column = player == 0 ? 11 : (player == 1 ? 3 : 21);
    renderNumber(score % 10000, 4, column, 24);
}

void renderShip(uint8_t x)
{
    SDL_Rect rect = {x * ZOOM, 216 * ZOOM, 13 * ZOOM, 8 * ZOOM};
    SDL_SetRenderDrawColor(renderer, 0xa0, 0x80, 0x40, 0x80);
    SDL_RenderFillRect(renderer,&rect);
    // SDL_RenderCopy(renderer, ship, NULL, &rect);
}

void renderLives(uint8_t lives)
{
    renderChar(lives, 8, 241);
    if (lives >= 2)
    {
        SDL_Rect shipRect = {64 * ZOOM, 241 * ZOOM, 13 * ZOOM, 8 * ZOOM};
        SDL_RenderCopy(renderer, ship, NULL, &shipRect);
    }
    if (lives >= 1)
    {
        SDL_Rect shipRect = {24 * ZOOM, 241 * ZOOM, 13 * ZOOM, 8 * ZOOM};
        SDL_RenderCopy(renderer, ship, NULL, &shipRect);
    }
}

void renderCredits(uint8_t credits)
{
    renderNumber(credits, 2, 24, 241);
}

int main(int argc, char *argv[])
{
    SDL_Event event;

    // Au début, il n'y avait rien
    init();

    // Charger des images
    background = IMG_LoadTexture(renderer, "./intro1.png");
    ship = IMG_LoadTexture(renderer, "./ship.png");
    chars = IMG_LoadTexture(renderer, "./letters.png");
    fprintf(stderr, "Error %s", IMG_GetError());

    // Initialiser plein de variables
    uint8_t player = 1;
    uint16_t scores[3] = {9999, 850, 0}; // HI, P1, P2
    uint8_t lives = 3;
    uint8_t credits = 9;

    // Faire des choses !
    int shipX = (WIDTH - 13) / 2;
    int dx = 0;
    int stop = 0;
    while (!stop)
    {
        // SDL_RenderCopy(renderer, background, NULL, NULL);
        renderText("SCORE<1> HI-SCORE SCORE<2>", 1, 0);
        renderText("0123456789012345678901234567", 0, 24);
        renderText("CREDIT", 17, 241);
        for (uint8_t i = 0; i < 3; i++)
        {
            renderScore(scores[i], i);
        }
        renderLives(lives);
        renderCredits(credits);
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
        if (dx != 0 && shipX + dx >= 0 && shipX + 13 + dx <= WIDTH)
        {
            shipX += dx;
        }
        SDL_Delay(10);
    }

    // Fin des haricots
    quit(EXIT_SUCCESS, NULL);
}
