#include "OFFLINE_int.h"
#include "../Menu/MENU_int.h"
#include "../Menu/MENU_draw.h"
#include "../../HAL/IR/IR_int.h"

u8 OFFLINE_u8Command(u8 *command)
{
    HIR_Command_t event;
    while (HIR_u8Poll(&event)) {
        if (event.Address == HIR_NEC_REMOTE_ADDRESS) {
            *command = event.Command;
            return 1;
        }
    }
    return 0;
}

u8 OFFLINE_u8Wait(u32 duration_us)
{
    u32 start = HIR_u32NowUS();
    u8 exit = 0, command;
    while ((u32)(HIR_u32NowUS() - start) < duration_us) {
        if (OFFLINE_u8Command(&command) && command == HIR_CMD_POWER) exit = 1;
    }
    while (OFFLINE_u8Command(&command))
        if (command == HIR_CMD_POWER) exit = 1;
    return exit;
}

u8 OFFLINE_u8Winner(const u8 *board, u8 rows, u8 cols, u8 run, u8 player)
{
    static const s8 dx[] = {1, 0, 1, 1};
    static const s8 dy[] = {0, 1, 1, -1};
    u8 row, col, direction, step;
    for (row = 0; row < rows; row++) {
        for (col = 0; col < cols; col++) {
            for (direction = 0; direction < 4; direction++) {
                for (step = 0; step < run; step++) {
                    s16 x = col + dx[direction] * step;
                    s16 y = row + dy[direction] * step;
                    if (x < 0 || x >= cols || y < 0 || y >= rows ||
                        board[y * cols + x] != player) break;
                }
                if (step == run) return 1;
            }
        }
    }
    return 0;
}

void OFFLINE_vRun(void)
{
    static const char *const games[] = {"CONNECT 4", "TIC TAC TOE", "SNAKE GAME"};
    u8 selection = 0, command, suppress_select = 1;
    HIR_vFlush();
    MENU_vList(games, 3, selection);
    while (1) {
        u8 previous = selection, action;
        if (!OFFLINE_u8Command(&command)) continue;
        /* Match M3: the mode confirmation's repeated full EQ frames must
         * not immediately launch a game. Any other command releases it. */
        if (command != HIR_CMD_EQ) suppress_select = 0;
        if (command == HIR_CMD_EQ && suppress_select) continue;
        action = MENU_u8Navigate(command, 3, &selection);
        if (action == 2) { HIR_vFlush(); return; }
        if (action == 1) {
            HIR_vFlush();
            if (selection == 0) OFFLINE_vConnect4();
            else if (selection == 1) OFFLINE_vTicTacToe();
            else OFFLINE_vSnake();
            HIR_vFlush();
            MENU_vList(games, 3, selection);
        } else if (previous != selection) {
            MENU_vItem(games[previous], previous, 0);
            MENU_vItem(games[selection], selection, 1);
        }
    }
}
