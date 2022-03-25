// gcc main.c $(sdl2-config --cflags --libs) -lSDL2_image
// gcc -std=c99 -o main -Wall -Wextra -pedantic main.c -lSDL2 -lSDL2_image
// ./main

// https://zestedesavoir.com/tutoriels/1014/utiliser-la-sdl-en-langage-c/

// https://blog.loadzero.com/blog/si78c/

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>

// #define WITH_SCREENSHOTS SDL_TRUE
#define ZOOM 3

#define GAME_WIDTH 224
#define GAME_HEIGHT 256

#define SHIP_WIDTH 13
#define SHIP_HEIGHT 8

#define ALIEN_WIDTH 8
#define ALIEN_HEIGHT 8
#define ALIEN_LINES 5
#define ALIEN_COLS 11
#define ALIEN_START_X (8 * 8)
#define ALIEN_START_Y (10 * 8)

#define SAUCER_WIDTH 16
#define SAUCER_HEIGHT 8

#define SCENE_NONE 0
#define SCENE_BOOT 1
#define SCENE_HOME 2
#define SCENE_PLAY 3
#define SCENE_GAME 4

// SDL stuff
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// Images
SDL_Texture *font = NULL;
SDL_Texture *ship = NULL;
SDL_Texture *aliens = NULL;
SDL_Texture *saucer = NULL;

// Game state
int32_t speed = 7;
int32_t delay;
uint8_t scene = SCENE_NONE;
uint32_t startTicks = 0L;
uint32_t step = 0L;
uint8_t playMode = 0; // 1=1 player, 2=2 players
uint8_t player = 1;
uint16_t scores[3] = {6789, 1010, 2020}; // HI, P1, P2
uint8_t lives = 3;
uint8_t credits = 9;
int shipX = (GAME_WIDTH - SHIP_WIDTH) / 2;
int shipDx = 0;
uint8_t fireX = 0;
uint8_t fireY = 0;
uint8_t alienMap[ALIEN_LINES][ALIEN_COLS];
uint8_t aliensX = 0;
uint8_t aliensY = 0;
uint8_t saucerX = 0;

SDL_bool flagLine = SDL_FALSE;
SDL_bool flagLives = SDL_FALSE;
SDL_bool flagShip = SDL_FALSE;
SDL_bool flagFire = SDL_FALSE;
SDL_bool flagAliens = SDL_FALSE;
SDL_bool flagSaucer = SDL_FALSE;

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
        GAME_WIDTH * ZOOM * 2, (GAME_HEIGHT + 16) * ZOOM,
        SDL_WINDOW_SHOWN);
    if (NULL == window)
    {
        quit(EXIT_FAILURE, "SDL_CreateWindow");
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // | SDL_RENDERER_PRESENTVSYNC);
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
 * @param column 0..2*!GAME_WIDTH/8-1
 * @param y      0..GAME_HEIGHT-1
 */
void renderChar(char c, uint8_t column, uint16_t y)
{
    if (c < 0 || c > 127 || column > 2 * GAME_WIDTH / 8 - 1 || y > (GAME_HEIGHT + 16 - 1))
    {
        fprintf(stderr, "renderChar: %c %d %d : char or position out of range!\r\n", c, column, y);
        return;
    }
    uint8_t offset_x = (c % 32) * 8;
    uint8_t offset_y = (c / 32) * 8;
    SDL_Rect src = {offset_x, offset_y, 8, 8};
    SDL_Rect dst = {column * 8 * ZOOM, y * ZOOM, 8 * ZOOM, 8 * ZOOM};
    // SDL_RenderFillRect(renderer, &dst);
    SDL_RenderCopy(renderer, font, &src, &dst);
}

/**
 * @brief Render some text at text column and pixel line
 *
 * @param s      text
 * @param column 0..GAME_WIDTH/8-1
 * @param line   0..GAME_HEIGHT-1
 */
void renderText(char *text, uint8_t column, uint8_t y)
{
    uint8_t i = 0;
    while (text[i])
    {
        renderChar(text[i], column + i, y);
        i += 1;
    }
}

/**
 * @brief Render debug line under game & screenshot
 *
 * @param sceneName BOOT/HOME/PLAY/GAME/...
 * @param duration  since last scene change
 */
void renderDebugText(char *sceneName, uint32_t duration)
{
    char flags[] = "_____";
    if (flagLine)
        flags[0] = 'L';
    if (flagShip)
        flags[1] = 'S';
    if (flagFire)
        flags[2] = 'F';
    if (flagAliens)
        flags[3] = 'A';
    if (flagSaucer)
        flags[4] = 'X';
    char buffer[256];
    sprintf(buffer,
            "%s %08.1f %s SHIP:%03d FIRE:%03d SPD/DLY:%02d/%02d",
            sceneName, duration / 1000.0, flags, shipX, fireY, speed, delay);
    uint8_t i = 0;
    while (buffer[i])
    {
        renderChar(buffer[i], 1 + i, GAME_HEIGHT);
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
 */
void renderScores()
{
    for (uint8_t i = 0; i < 3; i += 1)
    {
        // HI, P1, P2
        // Don't display P2 when P1 is playing & vice-versa
        if (scene != SCENE_GAME || (scene == SCENE_GAME && player == i))
        {
            //           1         2
            // 012345678901234567890123456789
            //  SCORE<1> HI-SCORE SCORE<2>
            //    0000    0000      0000   <- original
            //    0000     0000     0000   <- my version
            int column = i == 0 ? 12 : (i == 1 ? 3 : 21);
            renderNumber(scores[i] % 10000, 4, column, 3 * 8);
        }
    }
}

/**
 * @brief Render lives
 *
 * @param lives 1..3
 */
void renderLives(uint8_t lives)
{
    if (!flagLives)
        return;
    renderChar('0' + lives, 1, 30 * 8);
    for (uint8_t i = 0; i < lives - 1; i += 1)
    {
        SDL_Rect shipRect = {(26 + i * 16) * ZOOM, 30 * 8 * ZOOM,
                             SHIP_WIDTH * ZOOM, SHIP_HEIGHT * ZOOM};
        SDL_RenderCopy(renderer, ship, NULL, &shipRect);
    }
}

/**
 * @brief Render credits
 *
 * @param credits 0..99
 */
void renderCredits()
{
    renderNumber(credits, 2, 24, 30 * 8);
}

/**
 * @brief Render ship
 *
 * @param x Left position
 */
void renderShip()
{
    if (!flagShip)
        return;
    SDL_Rect rect = {shipX * ZOOM, 216 * ZOOM, SHIP_WIDTH * ZOOM, SHIP_HEIGHT * ZOOM};
    // fprintf(stderr, "SHIP: %d %d %d %d\n", rect.x, rect.y, rect.w, rect.h);
    // SDL_RenderFillRect(renderer, &rect);
    SDL_RenderCopy(renderer, ship, NULL, &rect);
}

/**
 * @brief Render fire
 *
 * @param x 0..GAME_WIDTH-1
 * @param y 0..GAME_HEIGHT-1
 */
void renderFire()
{
    if (!flagFire)
        return;
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
    SDL_Rect rect = {fireX * ZOOM, fireY * ZOOM, 1 * ZOOM, 2 * ZOOM};
    // fprintf(stderr, "FIRE: %d %d %d %d\n", rect.x, rect.y, rect.w, rect.h);
    SDL_RenderFillRect(renderer, &rect);
}

void renderGrid()
{
    // SDL_bool gx = SDL_TRUE;
    // SDL_bool gy = SDL_FALSE;
    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    for (uint16_t x = 0; x < GAME_WIDTH * 2; x += 8)
    {
        for (uint16_t y = 0; y < GAME_HEIGHT * 2; y += 8)
        {
            // if (gx || gy)
            if (x >= GAME_WIDTH)
            {
                SDL_SetRenderDrawColor(renderer, 0xc0, 0xc0, 0xc0, SDL_ALPHA_TRANSPARENT);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, SDL_ALPHA_TRANSPARENT);
            }
            SDL_Rect gridRect = {x * ZOOM, y * ZOOM, (x + 8) * ZOOM, (y + 8) * ZOOM};
            SDL_RenderDrawRect(renderer, &gridRect);
            // SDL_RenderFillRect(renderer, &gridRect);
            // gy = !gy;
        }
        // gx = !gx;
    }
    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void renderBottomLine()
{
    if (!flagLine)
        return;
    SDL_Rect lineRect = {0 * ZOOM, 239 * ZOOM, GAME_WIDTH * ZOOM, 1 * ZOOM};
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &lineRect);
}

/**
 * @brief Set the scene
 *
 * @param newScene SCENE_XXXX
 */
void setScene(uint8_t newScene)
{
    flagLine = SDL_FALSE;
    flagLives = SDL_FALSE;
    flagShip = SDL_FALSE;
    flagFire = SDL_FALSE;
    flagAliens = SDL_FALSE;
    flagSaucer = SDL_FALSE;

    switch (newScene)
    {
    case SCENE_BOOT:
        // display PLAY SPACE INVADERS
        scene = newScene;
        startTicks = SDL_GetTicks();
        break;
    case SCENE_HOME:
        // display "instructions" & aliens points
        scene = newScene;
        startTicks = SDL_GetTicks();
        break;
    case SCENE_PLAY:
        // start 1 player game (for now)
        scene = newScene;
        startTicks = SDL_GetTicks();
        playMode = 1;
        player = 1;
        break;
    case SCENE_GAME:
        // efectively play game
        scene = newScene;
        startTicks = SDL_GetTicks();
        shipX = (GAME_WIDTH - SHIP_WIDTH) / 2;
        shipDx = 0;
        flagLine = SDL_TRUE;
        flagLives = SDL_TRUE;
        flagShip = SDL_TRUE;
        flagAliens = SDL_TRUE;
        break;
    default:
        fprintf(stderr, "Unknown scene #%d\n", newScene);
        break;
    }
}

void renderScene()
{
    static uint32_t lastDuration = 0;
    uint32_t ticks = SDL_GetTicks();
    uint32_t duration = ticks - startTicks;

    // Clear screen to black
    SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    // Render "always here" items, except in BOOT scene
    if (scene != SCENE_BOOT)
    {
        renderText("SCORE<1> HI-SCORE SCORE<2>", 1, 1 * 8);
        renderScores(scores);
        renderLives(lives);
        renderText("CREDIT", 17, 30 * 8);
        renderCredits(credits);
    }

    switch (scene)
    {
    case SCENE_BOOT:
        renderDebugText("BOOT", duration);
        if (duration > 1000)
        {
            setScene(SCENE_HOME);
        }
        else
        {
            for (uint8_t line = 0; line < GAME_HEIGHT / 8; line++)
            {
                for (uint8_t col = 0; col < GAME_WIDTH / 8; col++)
                {
                    char c = rand() % 128;
                    renderChar(c, col, line * 8);
                }
            }
        }
        break;
    case SCENE_HOME:
        renderDebugText("HOME", duration);
        break;
    case SCENE_PLAY:
        renderDebugText("PLAY", duration);
        if (duration > 1000)
        {
            setScene(SCENE_GAME);
        }
        else
        {
            char buffer[256];
            sprintf(buffer, "PLAY PLAYER<%d>", player);
            renderText(buffer, 7, 14 * 8);
        }
        break;
    case SCENE_GAME:
        renderDebugText("GAME", duration);
        // Check if ship can/has to move
        if (shipDx != 0 && shipX + shipDx >= 8 && shipX + SHIP_WIDTH + shipDx <= GAME_WIDTH - 8)
        {
            shipX += shipDx;
        }
        // Check if ship touched by bomb
        // Check if fire can go up
        if (flagFire)
        {
            fireY -= 1;
            if (fireY < 32)
            {
                flagFire = 0;
            }
        }
        // Check if alien touched by fire
        if (flagFire)
        {
            // TODO
        }
        // ...
        break;
    default:
        renderDebugText("????", duration);
        break;
    }

    // Render conditional elements
    renderShip();
    renderFire();
    renderBottomLine();
    // renderAliens();
    // renderSaucer();
    // renderBuildings();
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

    fprintf(stderr, "char: %ld\n", sizeof(char));

    init();

    // Load images
    font = IMG_LoadTexture(renderer, "./font.png");
    ship = IMG_LoadTexture(renderer, "./ship.png");

    // Screenshots
    SDL_Texture *screenshots[3];
    uint8_t screenshot = 0;
    screenshots[0] = IMG_LoadTexture(renderer, "./intro1.png");
    screenshots[1] = IMG_LoadTexture(renderer, "./player-1.png");
    screenshots[2] = IMG_LoadTexture(renderer, "./game-1.png");
    // fprintf(stderr, "Error %s", IMG_GetError());
    SDL_bool grid = SDL_FALSE;

    setScene(SCENE_BOOT);

    SDL_bool stop = SDL_FALSE;
    while (!stop)
    {
        uint32_t beginTicks = SDL_GetTicks();
        renderScene();
        // Draw screenshot
        SDL_Rect screenshotRect = {GAME_WIDTH * ZOOM, 0, GAME_WIDTH * ZOOM, GAME_HEIGHT * ZOOM};
        SDL_RenderCopy(renderer, screenshots[screenshot], NULL, &screenshotRect);
        // Helps debugging item positions
        if (grid)
        {
            renderGrid();
        }
        SDL_RenderPresent(renderer);
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
            case SDLK_LEFT:
                if (flagShip)
                    shipDx = -1;
                break;
            case SDLK_RIGHT:
                if (flagShip)
                    shipDx = 1;
                break;
            case SDLK_SPACE:
                if (flagShip && !flagFire)
                {
                    flagFire = SDL_TRUE;
                    fireX = shipX + 6;
                    fireY = 216 - 3;
                }
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
                if (speed > 99)
                {
                    speed = 99;
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
                if (flagShip && shipDx < 0)
                    shipDx = 0;
                break;
            case SDLK_RIGHT:
                if (flagShip && shipDx > 0)
                    shipDx = 0;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        uint32_t endTicks = SDL_GetTicks();
        int32_t renderTicks = endTicks - beginTicks;
        delay = speed - renderTicks;
        if (delay < 0)
            delay = 0;
        if (delay > 100)
            delay = 100;
        SDL_Delay(delay);
    }

    // Fin des haricots
    quit(EXIT_SUCCESS, NULL);
}
