#include "OFFLINE_int.h"
#include "../Common/BOARD_ANIM.h"
#include "../Menu/MENU_draw.h"
#include "../../HAL/IR/IR_int.h"

/* Board drawing lives in BOARD_ANIM so the online Connect 4 reuses these
 * exact pixels. The wrapper keeps this file's original call sites. */
static void Cursor(u8 col, u16 color)
{
    BOARD_vColumnCursor(&BOARD_CONNECT4_LAYOUT, col, color);
}

void OFFLINE_vConnect4(void)
{
    u8 board[6][7], col, player, command;
    u8 restart = 1;
    while (1) {
        if (restart) {
            u8 r, c;
            for (r = 0; r < 6; r++)
                for (c = 0; c < 7; c++) board[r][c] = 0;
            col = 3;
            player = 1;
            restart = 0;
            HTFT_vFillBackgroundColor(TFT_BLACK);
            BOARD_vDrawGrid(&BOARD_CONNECT4_LAYOUT);
            MENU_vText(37, 5, "CONNECT 4", TFT_CYAN, TFT_BLACK, 1);
            Cursor(col, TFT_RED);
        }
        if (!OFFLINE_u8Command(&command)) continue;
        if (command == HIR_CMD_POWER) return;
        if (command == HIR_CMD_4 || command == HIR_CMD_6) {
            Cursor(col, TFT_BLACK);
            if (command == HIR_CMD_4 && col > 0) col--;
            if (command == HIR_CMD_6 && col < 6) col++;
            Cursor(col, player == 1 ? TFT_RED : TFT_YELLOW);
        }
        if (command == HIR_CMD_5) {
            s8 row = 5;
            u16 color = player == 1 ? TFT_RED : TFT_YELLOW;
            while (row >= 0 && board[row][col]) row--;
            if (row < 0) continue;
            board[row][col] = player;
            /* Reference places a rectangular coin immediately (no falling
             * animation). Keep its slot geometry and turn-color cursor. */
            BOARD_vToken(&BOARD_CONNECT4_LAYOUT, (u8)row, col, color);
            if (OFFLINE_u8Winner(&board[0][0], 6, 7, 4, player)) {
                MENU_vText(player == 1 ? 28 : 19, 142,
                           player == 1 ? "RED WINS!" : "YELLOW WINS!",
                           color, TFT_BLACK, 1);
                if (OFFLINE_u8Wait(2500000)) return;
                restart = 1;
            } else {
                /* As in M3, a full board without a win remains displayed;
                 * Power exits. Do not invent a different round policy. */
                player = 3 - player;
                Cursor(col, player == 1 ? TFT_RED : TFT_YELLOW);
            }
        }
    }
}
