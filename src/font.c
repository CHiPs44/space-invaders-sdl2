#include "../include/font.h"
#include "../include/const.h"
#include "../include/graphics.h"

SDL_Texture *font = NULL;

void initFont()
{
    font = IMG_LoadTexture(renderer, "./assets/font.png");
    if (font == NULL)
    {
        stopGraphics(EXIT_FAILURE, "initFont", SDL_GetError());
    }
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
    // Bitmap arrangement : 4 lines of 32 chars
    //   0123456789abcdef0123456789abcdef
    // 0 <control chars / weird stuff>
    // 2  !"#$%&'()*+,-./0123456789:;<=>?
    // 4 @ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_
    // 6 `abcdefghijklmnopqrstuvwxyz{|}~
    if (c < 0 || column > 2 * GAME_WIDTH / 8 - 1 || y > (GAME_HEIGHT + 16 - 1))
    {
        fprintf(stderr, "renderChar: %c %d %d : char or position out of range!\r\n", c, column, y);
        return;
    }
    uint8_t offset_x = (c % 32) * 8;
    uint8_t offset_y = (c / 32) * 8;
    SDL_Rect source =
        {offset_x, offset_y,
         8, 8};
    SDL_Rect destination =
        {offsetX + column * 8 * zoom, offsetY + y * zoom,
         8 * zoom, 8 * zoom};
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

// EOF
