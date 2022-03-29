// gcc main.c $(sdl2-config --cflags --libs) -lSDL2_image
// gcc -std=c99 -o main -Wall -Wextra -pedantic main.c -lSDL2 -lSDL2_image
// ./main

// https://zestedesavoir.com/tutoriels/1014/utiliser-la-sdl-en-langage-c/

// https://blog.loadzero.com/blog/si78c/

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>

#include "sprite.c"

// #define WITH_SCREENSHOTS SDL_TRUE
#define ZOOM 3

#define GAME_WIDTH 224
#define GAME_HEIGHT 256

#define SHIP_WIDTH 13
#define SHIP_HEIGHT 8
#define SHIP_Y 216

#define ALIEN_WIDTH 16
#define ALIEN_HEIGHT 8
#define ALIEN_LINES 5
#define ALIEN_COLUMNS 11
#define ALIEN_START_X (8 * 8)
#define ALIEN_START_Y (10 * 8)

#define SAUCER_WIDTH 16
#define SAUCER_HEIGHT 8
#define SAUCER_Y 40

#define ALIEN1_WIDTH 16
#define ALIEN1_HEIGHT 8

#define ALIEN2_WIDTH 16
#define ALIEN2_HEIGHT 8

#define ALIEN3_WIDTH 16
#define ALIEN3_HEIGHT 8

#define SHIELD_WIDTH 24
#define SHIELD_HEIGHT 16
#define SHIELD_Y 192

#define SCENE_NONE 0
#define SCENE_BOOT 1
#define SCENE_HOME 2
#define SCENE_PLAY 3
#define SCENE_GAME 4

// SDL stuff
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// Sprites
SDL_Texture *font = NULL;
Sprite *ship = NULL;
Sprite *fire = NULL;
// SDL_Texture *aliens = NULL;
Sprite *alien1 = NULL;
Sprite *alien2 = NULL;
Sprite *alien3 = NULL;
Sprite *saucer = NULL;
Sprite *shields[4] = {NULL, NULL, NULL, NULL};

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
int shipDx = 0;
uint8_t alienMap[ALIEN_LINES][ALIEN_COLUMNS];
uint8_t aliensX = 0;
uint8_t aliensY = 0;
int saucerExplode = 0;

SDL_bool flagLine = SDL_FALSE;
SDL_bool flagLives = SDL_FALSE;
SDL_bool flagShip = SDL_FALSE;
SDL_bool flagFire = SDL_FALSE;
SDL_bool flagAliens = SDL_FALSE;
SDL_bool flagSaucer = SDL_FALSE;
SDL_bool flagShields = SDL_FALSE;

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
 * @param column 0..2*GAME_WIDTH/8-1
 * @param y      0..GAME_HEIGHT-1
 */
void renderChar(char c, uint8_t column, uint16_t y)
{
    //   0123456789abcdef0123456789abcdef
    // 0 <control chars>
    // 2  !"#$%&'()*+,-./0123456789:;<=>?
    // 4 @ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_
    // 6 `abcdefghijklmnopqrstuvwxyz{|}~
    if (c < 0 || c > 127 || column > 2 * GAME_WIDTH / 8 - 1 || y > (GAME_HEIGHT + 16 - 1))
    {
        fprintf(stderr, "renderChar: %c %d %d : char or position out of range!\r\n", c, column, y);
        return;
    }
    uint8_t offset_x = (c % 32) * 8;
    uint8_t offset_y = (c / 32) * 8;
    SDL_Rect source = {offset_x, offset_y, 8, 8};
    SDL_Rect destination = {column * 8 * ZOOM, y * ZOOM, 8 * ZOOM, 8 * ZOOM};
    SDL_RenderCopy(renderer, font, &source, &destination);
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
    char flags[] = "lsfax-";
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
    if (saucerExplode)
        flags[5] = '*';
    char buffer[256];
    sprintf(buffer,
            "%s %08d %s SHIP:%03d FIRE:%03d SPD/DLY:%02d/%02d %d",
            sceneName, duration, flags,
            ship->rect.x, fire->rect.y,
            speed, delay, saucer->frame);
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
        // 0=HI, 1=P1, 2=P2
        // Don't display P2 when P1 is playing & vice-versa
        if (scene != SCENE_GAME || i == 0 || (scene == SCENE_GAME && player == i))
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
 */
void renderLives()
{
    if (!flagLives)
        return;
    renderChar('0' + lives, 1, 30 * 8);
    for (uint8_t i = 0; i < lives - 1; i += 1)
    {
        SDL_Rect rect = {(26 + i * 16) * ZOOM, 30 * 8 * ZOOM,
                         SHIP_WIDTH * ZOOM, SHIP_HEIGHT * ZOOM};
        SDL_RenderCopy(renderer, ship->texture, NULL, &rect);
    }
}

/**
 * @brief Render credits
 */
void renderCredits()
{
    renderNumber(credits, 2, 24, 30 * 8);
}

/**
 * @brief Render ship
 */
void renderShip()
{
    if (!flagShip)
        return;
    renderSprite(ship, renderer, ZOOM);
}

/**
 * @brief Render fire
 */
void renderFire()
{
    if (!flagFire)
        return;
    renderSprite(fire, renderer, ZOOM);
}

void renderSaucer()
{
    if (!flagSaucer)
        return;
    saucer->frame = 0;
    if (saucerExplode > 0)
    {
        if (saucerExplode < SDL_GetTicks())
        {
            saucer->frame = 1;
        }
        else
        {
            saucerExplode = 0;
        }
    }
    renderSprite(saucer, renderer, ZOOM);
}

void renderGrid()
{
    for (uint16_t x = 0; x < GAME_WIDTH * 2; x += 8)
    {
        if (x > GAME_WIDTH)
        {
            SDL_SetRenderDrawColor(renderer, 0xc0, 0xc0, 0xc0, SDL_ALPHA_OPAQUE);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, SDL_ALPHA_OPAQUE);
        }
        for (uint16_t y = 0; y < GAME_HEIGHT * 2; y += 8)
        {
            SDL_Rect rect = {
                x * ZOOM, y * ZOOM,
                (x + 8) * ZOOM, (y + 8) * ZOOM};
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}

void renderLine()
{
    if (!flagLine)
        return;
    SDL_Rect lineRect = {0 * ZOOM, 239 * ZOOM, GAME_WIDTH * ZOOM, 1 * ZOOM};
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &lineRect);
}

void renderShields()
{
    if (!flagShields)
        return;
    for (int i = 0; i < 4; i += 1)
    {
        renderSprite(shields[i], renderer, ZOOM);
    }
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
    flagShields = SDL_FALSE;

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
        ship->rect.x = (GAME_WIDTH - SHIP_WIDTH) / 2;
        shipDx = 0;
        flagLine = SDL_TRUE;
        flagLives = SDL_TRUE;
        flagShip = SDL_TRUE;
        flagAliens = SDL_TRUE;
        flagShields = SDL_TRUE;
        break;
    default:
        fprintf(stderr, "Unknown scene #%d\n", newScene);
        break;
    }
}

void renderScene()
{
    static uint32_t lastTicks = 0;
    uint32_t ticks = SDL_GetTicks();
    uint32_t duration = ticks - startTicks;

    // Clear screen to black
    SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    // Render "always here" items, except in BOOT scene
    if (scene != SCENE_BOOT)
    {
        renderText("SCORE<1> HI-SCORE SCORE<2>", 1, 1 * 8);
        renderScores();
        renderLives();
        renderText("CREDIT", 17, 30 * 8);
        renderCredits(credits);
    }

    switch (scene)
    {
    case SCENE_BOOT:
        renderDebugText("BOOT", duration);
        // Switch to HOME scene after 1 second
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
                    int c = rand() % (48 + 2 * duration);
                    if (c <= 255)
                    {
                        renderChar(c > 127 ? 127 : c, col, line * 8);
                    }
                }
            }
        }
        break;
    case SCENE_HOME:
        renderDebugText("HOME", duration);
        // WIP!
        renderText("PLAY", 12, 8 * 8);
        renderText("SPACE  INVADERS", 7, 11 * 8);
        renderText("*SCORE ADVANCE TABLE*", 3, 15 * 8);

        SDL_Rect sourceS = {0, 0, SAUCER_WIDTH, SAUCER_HEIGHT};
        SDL_Rect destinationS = {(9 - 2) * 8 * ZOOM, 17 * 8 * ZOOM, SAUCER_WIDTH * ZOOM, SAUCER_HEIGHT * ZOOM};
        SDL_RenderCopy(renderer, saucer->texture, &sourceS, &destinationS);
        renderText("=? MYSTERY", 9, 17 * 8);

        SDL_Rect source3 = {0, 0, ALIEN3_WIDTH, ALIEN3_HEIGHT};
        SDL_Rect destination3 = {(9 - 2) * 8 * ZOOM, 19 * 8 * ZOOM, ALIEN3_WIDTH * ZOOM, ALIEN3_HEIGHT * ZOOM};
        SDL_RenderCopy(renderer, alien3->texture, &source3, &destination3);
        renderText("=30 POINTS", 9, 19 * 8);

        SDL_Rect source2 = {0, 0, ALIEN2_WIDTH, ALIEN2_HEIGHT};
        SDL_Rect destination2 = {(9 - 2) * 8 * ZOOM, 21 * 8 * ZOOM, ALIEN2_WIDTH * ZOOM, ALIEN2_HEIGHT * ZOOM};
        SDL_RenderCopy(renderer, alien2->texture, &source2, &destination2);
        renderText("=20 POINTS", 9, 21 * 8);

        SDL_Rect source1 = {0, 0, ALIEN1_WIDTH, ALIEN1_HEIGHT};
        SDL_Rect destination1 = {(9 - 2) * 8 * ZOOM, 23 * 8 * ZOOM, ALIEN1_WIDTH * ZOOM, ALIEN1_HEIGHT * ZOOM};
        SDL_RenderCopy(renderer, alien1->texture, &source1, &destination1);
        renderText("=10 POINTS", 9, 23 * 8);
        break;
    case SCENE_PLAY:
        renderDebugText("PLAY", duration);
        // Switch to GAME scene after 1 second
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
        // // TEST : activate saucer all the time
        // if (!flagSaucer)
        // {
        //     flagSaucer = SDL_TRUE;
        //     saucer->rect.x = GAME_WIDTH + 1;
        // }
        if (flagSaucer)
        {
            saucer->rect.x -= 1;
            if (saucer->rect.x <= -16)
            {
                flagSaucer = SDL_FALSE;
            }
        }
        // Check if ship can/has to move
        if (shipDx != 0 && ship->rect.x + shipDx >= 8 && ship->rect.x + SHIP_WIDTH + shipDx <= GAME_WIDTH - 8)
        {
            ship->rect.x += shipDx;
        }
        // Check if ship touched by bombs
        // if (flagBomb)
        // {
        //     // TODO
        // }
        // Check if fire can go up
        if (flagFire)
        {
            fire->rect.y -= 1;
            if (fire->rect.y < 32)
            {
                // TODO explode player fire
                flagFire = SDL_FALSE;
            }
        }
        // Check if saucer touched by fire
        if (flagSaucer &&
            flagFire &&
            fire->rect.y <= SAUCER_Y + SAUCER_HEIGHT &&
            fire->rect.x >= saucer->rect.x &&
            fire->rect.x <= saucer->rect.x + SAUCER_WIDTH)
        {
            saucerExplode = SDL_GetTicks() + 250;
            scores[player] += 50;
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
    // renderAliens();
    renderSaucer();
    renderShields();
    renderLine();
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

    // fprintf(stderr, "char: %ld\n", sizeof(char));

    init();

    // Load images
    font = IMG_LoadTexture(renderer, "./font.png");
    ship = createSprite(renderer, "./ship.png", 1);
    ship->rect.y = SHIP_Y;
    fire = createSprite(renderer, "./fire.png", 1);
    alien1 = createSprite(renderer, "./alien1.png", 2);
    alien2 = createSprite(renderer, "./alien2.png", 2);
    alien3 = createSprite(renderer, "./alien3.png", 2);
    saucer = createSprite(renderer, "./saucer.png", 2);
    saucer->rect.y = SAUCER_Y;
    for (int i = 0; i < 4; i += 1)
    {
        shields[i] = createSprite(renderer, "./shield.png", 1);
        // TODO find exact location (32 is good for #1)
        shields[i]->rect.x = 32 + i * 40;
        shields[i]->rect.y = SHIELD_Y;
    }

    // Screenshots
    SDL_Texture *screenshots[3];
    uint8_t screenshot = 2;
    screenshots[0] = IMG_LoadTexture(renderer, "./intro1.png");
    screenshots[1] = IMG_LoadTexture(renderer, "./player-1.png");
    screenshots[2] = IMG_LoadTexture(renderer, "./game-1.png");
    // fprintf(stderr, "Error %s", IMG_GetError());
    SDL_bool grid = SDL_FALSE;

    setScene(SCENE_BOOT);

    SDL_bool stop = SDL_FALSE;
    while (!stop)
    {
        uint32_t frameBegin = SDL_GetTicks();
        // Draw current scene
        renderScene();
        // Draw screenshot
        SDL_Rect screenshotRect = {GAME_WIDTH * ZOOM, 0, GAME_WIDTH * ZOOM, GAME_HEIGHT * ZOOM};
        SDL_RenderCopy(renderer, screenshots[screenshot], NULL, &screenshotRect);
        // Helps debugging item positions
        if (grid)
            renderGrid();
        // Render complete screen
        SDL_RenderPresent(renderer);
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
                    fire->rect.x = ship->rect.x + SHIP_WIDTH / 2;
                    fire->rect.y = ship->rect.y - fire->rect.w;
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
        uint32_t renderEnd = SDL_GetTicks();
        int32_t renderTicks = renderEnd - frameBegin;
        delay = speed - renderTicks;
        if (delay < 0)
            delay = 0;
        if (delay > 100)
            delay = 100;
        SDL_Delay(delay);
        uint32_t frameEnd = SDL_GetTicks();
        uint32_t frameDuration = frameEnd - frameBegin;
    }

    // Fin des haricots
    quit(EXIT_SUCCESS, NULL);
}
