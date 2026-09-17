/*
 * Connect4_Animation_prg.c
 *
 * Online Connect 4 TFT renderer using shared BOARD_ANIM.
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#include "../../../LIB/STD_TYPES.h"

#include "../../../HAL/TFT/TFT_int.h"
#include "../../../MCAL/SYSTICK/SYSTICK_int.h"

#include "../../../APP/Common/BOARD_ANIM.h"
#include "../../../APP/Common/FONT3x5.h"
#include "../../../APP/Common/LOADING_ANIM.h"
#include "../../Menu/MENU_draw.h"

#include "Connect4_Animation.h"

static u8 G_u8BoardDrawn = 0;
static Connect4_Mark G_xLastTurn = CONNECT4_MARK_NONE;

static Connect4_Mark ParseMark(char ch)
{
	if (ch == 'R') return CONNECT4_MARK_RED;
	if (ch == 'Y') return CONNECT4_MARK_YELLOW;
	return CONNECT4_MARK_NONE;
}

static u8 CellIsValid(u8 row, u8 col)
{
	return row < CONNECT4_ROWS && col < CONNECT4_COLS;
}

static void DrawToken(u8 row, u8 col, Connect4_Mark mark)
{
	u16 color = (mark == CONNECT4_MARK_RED) ? TFT_RED : TFT_YELLOW;
	BOARD_vToken(&BOARD_CONNECT4_LAYOUT, row, col, color);
}

static void DrawColumnCursor(u8 col, u16 color)
{
	BOARD_vColumnCursor(&BOARD_CONNECT4_LAYOUT, col, color);
}

/* Win detection using the same algorithm as OFFLINE_u8Winner */
static u8 DetectWin(const char *board, u8 lastRow, u8 lastCol)
{
	Connect4_Mark player = (board[lastRow * CONNECT4_COLS + lastCol] == 'R')
	                      ? CONNECT4_MARK_RED : CONNECT4_MARK_YELLOW;
	u8 dir[4][2] = {{1,0}, {0,1}, {1,1}, {1,-1}}; /* dr, dc */

	for (u8 d = 0; d < 4; d++) {
		u8 r = lastRow, c = lastCol, cnt = 1;
		/* forward */
		while (CellIsValid(r + dir[d][0], c + dir[d][1]) &&
		       (board[(r + dir[d][0]) * CONNECT4_COLS + c + dir[d][1]] == 
		        (player == CONNECT4_MARK_RED ? 'R' : 'Y'))) {
			r += dir[d][0]; c += dir[d][1]; cnt++;
		}
		/* backward */
		r = lastRow; c = lastCol;
		while (CellIsValid(r - dir[d][0], c - dir[d][1]) &&
		       (board[(r - dir[d][0]) * CONNECT4_COLS + c - dir[d][1]] ==
		        (player == CONNECT4_MARK_RED ? 'R' : 'Y'))) {
			r -= dir[d][0]; c -= dir[d][1]; cnt++;
		}
		if (cnt >= 4) return 1;
	}
	return 0;
}

/* Draw win line through the winning 4 tokens */
static void DrawWinLine(u8 rowA, u8 colA, u8 rowB, u8 colB)
{
	BOARD_vWinLine(&BOARD_CONNECT4_LAYOUT, rowA, colA, rowB, colB, TFT_WHITE);
}

void Connect4_Animation_vStart(void)
{
	G_u8BoardDrawn = 0;
	G_xLastTurn = CONNECT4_MARK_NONE;
	HTFT_vFillBackgroundColor(TFT_BLACK);
	BOARD_vDrawGrid(&BOARD_CONNECT4_LAYOUT);
	LOADING_ANIM_vPlay();
}

void Connect4_Animation_vShowState(const Connect4_ServerState *state)
{
	u8 col;

	if (state == 0 || (!state->Alive && state->Status != CONNECT4_STATUS_ENDED)) {
		return;
	}

	if (!G_u8BoardDrawn || state->Turn != G_xLastTurn) {
		if (!G_u8BoardDrawn) {
			HTFT_vFillBackgroundColor(TFT_BLACK);
			BOARD_vDrawGrid(&BOARD_CONNECT4_LAYOUT);
		}
		G_xLastTurn = state->Turn;

		for (u8 r = 0; r < CONNECT4_ROWS; r++) {
			for (col = 0; col < CONNECT4_COLS; col++) {
				char ch = state->Board[r * CONNECT4_COLS + col];
				Connect4_Mark mark = ParseMark(ch);
				if (mark != CONNECT4_MARK_NONE) {
					DrawToken(r, col, mark);
				}
			}
		}

		/* Draw column cursor for current player */
		DrawColumnCursor(col, (state->Turn == CONNECT4_MARK_RED) ? TFT_RED : TFT_YELLOW);

		/* Win line if needed */
		if (state->Winner == CONNECT4_WINNER_P1 || state->Winner == CONNECT4_WINNER_P2) {
			u8 wrow = state->LastMoveRow, wcol = 0;
			for (u8 i = 0; i < CONNECT4_CELL_COUNT; i++) {
				if (state->Board[i] == (state->Winner == CONNECT4_WINNER_P1 ? 'R' : 'Y')) {
					wcol = i % CONNECT4_COLS;
					if (i >= CONNECT4_CELL_COUNT - CONNECT4_COLS) break;
					u8 nextSame = 0;
					for (u8 j = 1; j < 4 && i + j * CONNECT4_COLS < CONNECT4_CELL_COUNT; j++) {
						if (state->Board[i + j * CONNECT4_COLS] == state->Board[i]) nextSame++;
					}
					if (nextSame >= 3) { wrow = i / CONNECT4_COLS; break; }
				}
			}
			DrawWinLine(wrow, wcol, state->LastMoveRow, state->LastMoveCol);
		}
		G_u8BoardDrawn = 1;
	}
}

void Connect4_Animation_vShowIdle(void)
{
	HTFT_vFillBackgroundColor(TFT_BLACK);
	BOARD_vDrawGrid(&BOARD_CONNECT4_LAYOUT);
	MENU_vText(25, 5, "CONNECT 4", TFT_CYAN, TFT_BLACK, 1);
	MENU_vText(30, 151, "WAITING", TFT_GRAY, TFT_BLACK, 1);
}