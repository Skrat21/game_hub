#include "OFFLINE_int.h"
#include "../Common/BOARD_ANIM.h"
#include "../Menu/MENU_draw.h"
#include "../../HAL/IR/IR_int.h"

/* Board drawing lives in BOARD_ANIM so the online XO reuses these exact
 * pixels. The wrappers keep this file's original call sites unchanged. */
static void Cursor(u8 row, u8 col, u16 color)
{
    BOARD_vCursor(&BOARD_XO_LAYOUT, row, col, color);
}

static void Cell(u8 row, u8 col, u8 player)
{
    if (!player) {
        BOARD_vCell(&BOARD_XO_LAYOUT, row, col, TFT_BLACK);
    } else {
        BOARD_vMark(&BOARD_XO_LAYOUT, row, col, player == 1 ? 'X' : 'O',
                    player == 1 ? TFT_RED : TFT_YELLOW);
    }
}

void OFFLINE_vTicTacToe(void)
{
    u8 board[3][3], row, col, player, moves, command;
    u8 restart = 1;
    while (1) {
        if (restart) {
            u8 r, c;
            for (r = 0; r < 3; r++)
                for (c = 0; c < 3; c++) board[r][c] = 0;
            row = col = 1;
            player = 1;
            moves = 0;
            restart = 0;
            HTFT_vFillBackgroundColor(TFT_BLACK);
            MENU_vText(31, 143, "TIC TAC TOE", TFT_CYAN, TFT_BLACK, 1);
            BOARD_vDrawGrid(&BOARD_XO_LAYOUT);
            Cursor(row, col, TFT_CYAN);
        }
        if (!OFFLINE_u8Command(&command)) continue;
        if (command == HIR_CMD_POWER) return;
        if (command == HIR_CMD_4 || command == HIR_CMD_6 ||
            command == HIR_CMD_2 || command == HIR_CMD_8) {
            Cell(row, col, board[row][col]);
            Cursor(row, col, TFT_WHITE);
            if (command == HIR_CMD_4 && col > 0) col--;
            if (command == HIR_CMD_6 && col < 2) col++;
            if (command == HIR_CMD_2 && row > 0) row--;
            if (command == HIR_CMD_8 && row < 2) row++;
            Cursor(row, col, TFT_CYAN);
        }
        if (command == HIR_CMD_5 && !board[row][col]) {
            u8 win;
            board[row][col] = player;
            moves++;
            Cell(row, col, player);
            Cursor(row, col, TFT_CYAN);
            win = OFFLINE_u8Winner(&board[0][0], 3, 3, 3, player);
            if (win || moves == 9) {
                MENU_vText(35, 10, win ? (player == 1 ? "X WINS" : "O WINS") : "DRAW",
                           win ? (player == 1 ? TFT_RED : TFT_YELLOW) : TFT_CYAN,
                           TFT_BLACK, 1);
                if (OFFLINE_u8Wait(win ? 2500000 : 1500000)) return;
                restart = 1;
            } else player = 3 - player;
        }
    }
}
