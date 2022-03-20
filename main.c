// gcc main.c $(sdl2-config --cflags --libs) -lSDL2_image
// gcc -std=c99 -o main -Wall -Wextra -pedantic main.c -lSDL2 -lSDL2_image
// ./main

// https://zestedesavoir.com/tutoriels/1014/utiliser-la-sdl-en-langage-c/

// https://blog.loadzero.com/blog/si78c/

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>

#define GAME_WIDTH 224
#define GAME_HEIGHT 256
#define ZOOM 3

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *backgrounds[3] = {NULL, NULL, NULL};
uint8_t background = 0;
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
    sprintf(title, "Space Invaders 1978 (%dx%dx%d)", GAME_WIDTH, GAME_HEIGHT, ZOOM);
    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        GAME_WIDTH * ZOOM * 2, GAME_HEIGHT * ZOOM,
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

/**
 * @brief Render one char at text column and pixel line
 *
 * @param c      char to render
 * @param column 0..GAME_WIDTH/8-1
 * @param y      0..GAME_HEIGHT-1
 */
void renderChar(char c, uint8_t column, uint8_t y)
{
    if (c < 32 || c > 127 || column > GAME_WIDTH / 8 - 1 || y > GAME_HEIGHT - 1)
    {
        fprintf(stderr, "renderChar: %c %d %d : char or position out of range!\r\n", c, column, y);
        return;
    }
    uint8_t offset_x = (c % 32) * 8;
    uint8_t offset_y = (c / 32) * 8;
    SDL_Rect src = {offset_x, offset_y, 8, 8};
    SDL_Rect dst = {column * 8 * ZOOM, y * ZOOM, 8 * ZOOM, 8 * ZOOM};
    // SDL_RenderFillRect(renderer, &dst);
    SDL_RenderCopy(renderer, chars, &src, &dst);
}

/**
 * @brief Render some text at text column and pixel line
 *
 * @param s      text
 * @param column 0..GAME_WIDTH/8-1
 * @param line   0..GAME_HEIGHT-1
 */
void renderText(char *s, uint8_t column, uint8_t y)
{
    uint8_t i = 0;
    while (s[i])
    {
        renderChar(s[i], column + i, y);
        i += 1;
    }
}

/**
 * @brief Render number at text column and pixel line
 *
 * @param number Number to display
 * @param width  Width filled with zero padding
 * @param column 0..GAME_WIDTH/8-1
 * @param line   0..GAME_HEIGHT-1
 */
void renderNumber(uint16_t number, uint8_t width, uint8_t column, uint8_t y)
{
    char buffer[width + 1];
    sprintf(buffer, "%0*d", width, number);
    renderText(buffer, column, y);
}

/**
 * @brief Render high score player's one, two
 *
 * @param score  Score to display
 * @param player 0=high, 1=player 1, 2=player 2
 */
void renderScore(uint16_t score, uint8_t player)
{
    int column = player == 0 ? 11 : (player == 1 ? 3 : 21);
    renderNumber(score % 10000, 4, column, 3 * 8);
}

/**
 * @brief Render left lives
 *
 * @param lives 1..3
 */
void renderLives(uint8_t lives)
{
    renderChar('0' + lives, 1, 240);
    if (lives >= 3)
    {
        SDL_Rect shipRect = {57 * ZOOM, 240 * ZOOM, 13 * ZOOM, 8 * ZOOM};
        SDL_RenderCopy(renderer, ship, NULL, &shipRect);
    }
    if (lives >= 2)
    {
        SDL_Rect shipRect = {41 * ZOOM, 240 * ZOOM, 13 * ZOOM, 8 * ZOOM};
        SDL_RenderCopy(renderer, ship, NULL, &shipRect);
    }
    if (lives >= 1)
    {
        SDL_Rect shipRect = {25 * ZOOM, 240 * ZOOM, 13 * ZOOM, 8 * ZOOM};
        SDL_RenderCopy(renderer, ship, NULL, &shipRect);
    }
}

/**
 * @brief Render credits
 *
 * @param credits
 */
void renderCredits(uint8_t credits)
{
    renderNumber(credits, 2, 24, 240);
}

/**
 * @brief Render ship
 *
 * @param x Left position
 */
void renderShip(uint8_t x)
{
    SDL_Rect rect = {x * ZOOM, 216 * ZOOM, 13 * ZOOM, 8 * ZOOM};
    // SDL_RenderFillRect(renderer, &rect);
    SDL_RenderCopy(renderer, ship, NULL, &rect);
}

/**
 * @brief Render fire
 *
 * @param x 0..GAME_WIDTH-1
 * @param y 0..GAME_HEIGHT-1
 */
void renderFire(uint8_t x, uint8_t y)
{
    SDL_Rect rect = {x * ZOOM, y * ZOOM, 1 * ZOOM, 2 * ZOOM};
    SDL_RenderFillRect(renderer, &rect);
}

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

    init();

    backgrounds[0] = IMG_LoadTexture(renderer, "./intro1.png");
    backgrounds[1] = IMG_LoadTexture(renderer, "./player-1.png");
    backgrounds[2] = IMG_LoadTexture(renderer, "./game-1.png");
    ship = IMG_LoadTexture(renderer, "./ship.png");
    chars = IMG_LoadTexture(renderer, "./letters.png");
    fprintf(stderr, "Error %s", IMG_GetError());

    uint8_t player = 1;
    uint16_t scores[3] = {9999, 850, 0}; // HI, P1, P2
    uint8_t lives = 3;
    uint8_t credits = 9;

    int ship_x = (GAME_WIDTH - 13) / 2;
    int dx = 0;
    int stop = 0;
    int grid = 0;
    while (!stop)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_Rect backgroundRect = {GAME_WIDTH * ZOOM, 0, GAME_WIDTH * ZOOM, GAME_HEIGHT * ZOOM};
        SDL_RenderCopy(renderer, backgrounds[background], NULL, &backgroundRect);
        renderText("SCORE<1> HI-SCORE SCORE<2>", 2, 8);
        renderText("0123456789012345678901234567", 0, 40);
        renderText("CREDIT", 17, 240);
        for (uint8_t i = 0; i < 3; i++)
        {
            renderScore(scores[i], i);
        }
        renderShip(ship_x);
        SDL_Rect lineRect = {0 * ZOOM, 239 * ZOOM, GAME_WIDTH * ZOOM, 1 * ZOOM};
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderFillRect(renderer, &lineRect);
        renderLives(lives);
        renderCredits(credits);
        if (grid)
        {
            SDL_bool gx = SDL_TRUE;
            SDL_bool gy = SDL_FALSE;
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            for (uint16_t x = 0; x < GAME_WIDTH * 2; x += 8)
            {
                for (uint16_t y = 0; y < GAME_HEIGHT * 2; y += 8)
                {
                    if (gx || gy)
                    {
                        SDL_SetRenderDrawColor(renderer, 0xc0, 0xc0, 0xc0, 0x10);
                    }
                    else
                    {
                        SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, 0x10);
                    }
                    SDL_Rect gridRect = {x * ZOOM, y * ZOOM, (x + 8) * ZOOM, (y + 8) * ZOOM};
                    SDL_RenderDrawRect(renderer, &gridRect);
                    // SDL_RenderFillRect(renderer, &gridRect);
                    gy = !gy;
                }
                gx = !gx;
            }
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        }
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
            case SDLK_KP_PLUS:
                background = (background + 1) % 3;
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
        if (dx != 0 && ship_x + dx >= 0 && ship_x + 13 + dx <= GAME_WIDTH)
        {
            ship_x += dx;
        }
        SDL_Delay(10);
    }

    // Fin des haricots
    quit(EXIT_SUCCESS, NULL);
}
