#include "OFFLINE_int.h"
#include "../Menu/MENU_draw.h"
#include "../../HAL/IR/IR_int.h"

#define SNAKE_CAPACITY 288
static struct {
    u8 x[SNAKE_CAPACITY], y[SNAKE_CAPACITY];
    u16 length; /* M3 used u8: widened to safely cover all 16*18 cells. */
    u8 direction, requested, food_x, food_y;
    u32 random;
} G_xSnake;
static const s8 G_s8Dx[] = {0, 1, 0, -1};
static const s8 G_s8Dy[] = {-1, 0, 1, 0};

static u32 Random(void)
{
    u32 value = G_xSnake.random;
    value ^= value << 13;
    value ^= value >> 17;
    value ^= value << 5;
    G_xSnake.random = value;
    return value;
}
static u8 Occupied(u8 x, u8 y)
{
    u16 i;
    for (i = 0; i < G_xSnake.length; i++)
        if (G_xSnake.x[i] == x && G_xSnake.y[i] == y) return 1;
    return 0;
}
static void Cell(u8 x, u8 y, u16 color)
{
    MENU_vRect(x * 8 + 1, y * 8 + 1, 7, 7, color);
}
static void Food(void)
{
    u16 free_cells = SNAKE_CAPACITY - G_xSnake.length;
    u16 target;
    u8 x, y;
    if (!free_cells) return;
    target = Random() % free_cells;
    for (y = 0; y < 18; y++) {
        for (x = 0; x < 16; x++) {
            if (!Occupied(x, y)) {
                if (!target) {
                    G_xSnake.food_x = x;
                    G_xSnake.food_y = y;
                    Cell(x, y, TFT_RED);
                    return;
                }
                target--;
            }
        }
    }
}
static void ResetRound(void)
{
    static const u8 start_x[] = {6, 5, 6, 7};
    static const u8 start_y[] = {5, 5, 4, 5};
    u8 i, direction = Random() % 4;
    G_xSnake.length = 3;
    G_xSnake.direction = G_xSnake.requested = direction;
    HTFT_vFillBackgroundColor(TFT_BLACK);
    MENU_vText(49, 148, "SNAKE", TFT_CYAN, TFT_BLACK, 1);
    MENU_vRect(0, 145, 128, 1, TFT_WHITE);
    for (i = 0; i < 3; i++) {
        G_xSnake.x[i] = start_x[direction] - i * G_s8Dx[direction];
        G_xSnake.y[i] = start_y[direction] - i * G_s8Dy[direction];
        Cell(G_xSnake.x[i], G_xSnake.y[i], TFT_GREEN);
    }
    Food();
}
static void Move(void)
{
    s16 x, y;
    u16 i;
    u8 ate;
    G_xSnake.direction = G_xSnake.requested;
    x = G_xSnake.x[0] + G_s8Dx[G_xSnake.direction];
    y = G_xSnake.y[0] + G_s8Dy[G_xSnake.direction];
    /* Preserve reference collision semantics, including occupied tail. */
    if (x < 0 || x >= 16 || y < 0 || y >= 18 || Occupied(x, y)) {
        ResetRound();
        return;
    }
    ate = x == G_xSnake.food_x && y == G_xSnake.food_y;
    if (ate) G_xSnake.length++;
    else Cell(G_xSnake.x[G_xSnake.length - 1], G_xSnake.y[G_xSnake.length - 1], TFT_BLACK);
    for (i = G_xSnake.length - 1; i > 0; i--) {
        G_xSnake.x[i] = G_xSnake.x[i - 1];
        G_xSnake.y[i] = G_xSnake.y[i - 1];
    }
    G_xSnake.x[0] = x;
    G_xSnake.y[0] = y;
    Cell(x, y, TFT_GREEN);
    /* Full board: no free food cell; hold until the next collision/reset. */
    if (ate && G_xSnake.length < SNAKE_CAPACITY) Food();
}
void OFFLINE_vSnake(void)
{
    u32 previous;
    u8 command;
    G_xSnake.random = HIR_u32NowUS();
    if (!G_xSnake.random) G_xSnake.random = 1;
    ResetRound();
    previous = HIR_u32NowUS();
    while (1) {
        if (OFFLINE_u8Command(&command)) {
            u8 direction = 4;
            if (command == HIR_CMD_POWER) return;
            if (command == HIR_CMD_2) direction = 0;
            if (command == HIR_CMD_6) direction = 1;
            if (command == HIR_CMD_8) direction = 2;
            if (command == HIR_CMD_4) direction = 3;
            if (direction < 4 && direction != (G_xSnake.direction + 2) % 4)
                G_xSnake.requested = direction;
        }
        if ((u32)(HIR_u32NowUS() - previous) >= 250000) {
            previous += 250000;
            Move();
        }
    }
}
