#include "MENU_draw.h"

/* Visual asset: the reference's uppercase 5x7 glyph shapes (not its TFT
 * driver). Only the characters used by the hub are stored. */
static const u8 G_u8Letters[26][5] = {
    {126,17,17,17,126},{127,73,73,73,54},{62,65,65,65,34},
    {127,65,65,34,28},{127,73,73,73,65},{127,9,9,9,1},
    {62,65,73,73,122},{127,8,8,8,127},{0,65,127,65,0},
    {32,64,65,63,1},{127,8,20,34,65},{127,64,64,64,64},
    {127,2,12,2,127},{127,4,8,16,127},{62,65,65,65,62},
    {127,9,9,9,6},{62,65,81,33,94},{127,9,25,41,70},
    {70,73,73,73,49},{1,1,127,1,1},{63,64,64,64,63},
    {31,32,64,32,31},{63,64,56,64,63},{99,20,8,20,99},
    {7,8,112,8,7},{97,81,73,69,67}
};
static const u8 G_u8Digits[10][5] = {
    {62,81,73,69,62},{0,66,127,64,0},{66,97,81,73,70},
    {33,65,69,75,49},{24,20,18,127,16},{39,69,69,69,57},
    {60,74,73,73,48},{1,113,9,5,3},{54,73,73,73,54},{6,73,73,41,30}
};

void MENU_vRect(u16 x, u16 y, u16 width, u16 height, u16 color)
{
    if (!width || !height || x >= 128 || y >= 160)
        return;
    if (width > 128 - x) width = 128 - x;
    if (height > 160 - y) height = 160 - y;
    HTFT_vSetXPos(x, x + width - 1);
    HTFT_vSetYPos(y, y + height - 1);
    HTFT_vFillRectangle(color);
}

void MENU_vText(u16 x, u16 y, const char *text, u16 color, u16 background, u8 scale)
{
    u16 length = 0, index;
    if (!text || !scale) return;
    while (text[length]) length++;
    /* The M3 artwork rotates each glyph AND string order by 180 degrees.
     * Preserve those panel coordinates; do not change M4 TFT orientation. */
    for (index = 0; index < length; index++) {
        const u8 *glyph = 0;
        char ch = text[index];
        u16 gx = x + (length - index - 1) * 6 * scale;
        u8 col, row;
        if (ch >= 'A' && ch <= 'Z') glyph = G_u8Letters[ch - 'A'];
        if (ch >= '0' && ch <= '9') glyph = G_u8Digits[ch - '0'];
        MENU_vRect(gx, y, 6 * scale, 8 * scale, background);
        for (col = 0; col < 5; col++) {
            u8 bits = glyph ? glyph[col] : ((ch == '!' && col == 2) ? 95 : 0);
            for (row = 0; row < 8; row++)
                if (bits & (1U << row))
                    MENU_vRect(gx + (5 - col) * scale, y + (7 - row) * scale,
                               scale, scale, color);
        }
    }
}

void MENU_vGrid(u16 x, u16 y, u16 width, u16 height, u8 cols, u8 rows)
{
    u8 i;
    MENU_vRect(x, y, width + 1, height + 1, TFT_BLACK);
    for (i = 0; i <= cols; i++)
        MENU_vRect(x + i * (width / cols), y, 1, height + 1, TFT_WHITE);
    for (i = 0; i <= rows; i++)
        MENU_vRect(x, y + i * (height / rows), width + 1, 1, TFT_WHITE);
}

void MENU_vItem(const char *text, u8 index, u8 highlighted)
{
    u16 length = 0, y = 125 - index * 16;
    u16 bg = highlighted ? TFT_CYAN : TFT_BLACK;
    while (text[length]) length++;
    MENU_vRect(5, y - 3, 118, 16, bg);
    MENU_vText((128 - length * 6) / 2, y, text,
               highlighted ? TFT_BLACK : TFT_WHITE, bg, 1);
}

void MENU_vList(const char *const *items, u8 count, u8 selection)
{
    u8 i;
    HTFT_vFillBackgroundColor(TFT_BLACK);
    MENU_vText(28, 143, "GAME CONSOLE", TFT_CYAN, TFT_BLACK, 1);
    MENU_vRect(10, 140, 109, 1, TFT_CYAN);
    for (i = 0; i < count; i++) MENU_vItem(items[i], i, i == selection);
}
