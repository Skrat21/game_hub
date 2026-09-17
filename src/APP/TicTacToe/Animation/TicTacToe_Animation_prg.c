/*
 * TicTacToe_Animation_prg.c
 *
 * Online Tic-Tac-Toe (XO) TFT renderer using shared BOARD_ANIM.
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

#include "TicTacToe_Animation.h"

#include "../../Menu/MENU_draw.h"

static u8 G_u8BoardDrawn = 0;
static XO_Mark G_xLastTurn = XO_MARK_NONE;

static void DrawCell(u8 row, u8 col, XO_Mark mark)
{
	if (mark == XO_MARK_NONE) {
		BOARD_vCell(&BOARD_XO_LAYOUT, row, col, TFT_BLACK);
	} else {
		BOARD_vMark(&BOARD_XO_LAYOUT, row, col,
			mark == XO_MARK_X ? 'X' : 'O',
			mark == XO_MARK_X ? TFT_RED : TFT_YELLOW);
	}
}

static void DrawWinLine(XO_Winner winner)
{
	static const struct { u8 row, col; } lines[8][2] = {
		/* rows */
		{{0,0},{0,2}}, {{1,0},{1,2}}, {{2,0},{2,2}},
		/* cols */
		{{0,0},{2,0}}, {{0,1},{2,1}}, {{0,2},{2,2}},
		/* diagonals */
		{{0,0},{2,2}}, {{0,2},{2,0}}
	};

	u8 color = (winner == XO_WINNER_X) ? TFT_RED :
	           (winner == XO_WINNER_O) ? TFT_YELLOW : TFT_CYAN;

	for (u8 i = 0; i < 8; i++) {
		if (winner == XO_WINNER_X || winner == XO_WINNER_O) {
			if (i != 2 && i != 5 && i != 6 && i != 7) continue;
			if (winner != XO_WINNER_DRAW) {
				if (!(i < 3 || i == 6)) continue; /* X wins on lines 0,1,2,6,7 */
				if (i == 6 || i == 7) {
					/* Only one diagonal winner at a time */
				}
			}
		}
		if (winner == XO_WINNER_DRAW) break; /* no win line */
		BOARD_vWinLine(&BOARD_XO_LAYOUT,
			lines[i][0].row, lines[i][0].col,
			lines[i][1].row, lines[i][1].col, color);
	}

	if (winner == XO_WINNER_DRAW) {
		BOARD_vWinLine(&BOARD_XO_LAYOUT, 1, 0, 1, 2, TFT_CYAN);
	}
}

void XO_Animation_vStart(void)
{
	G_u8BoardDrawn = 0;
	G_xLastTurn = XO_MARK_NONE;
	HTFT_vFillBackgroundColor(TFT_BLACK);
	BOARD_vDrawGrid(&BOARD_XO_LAYOUT);
	LOADING_ANIM_vPlay();
}

void XO_Animation_vShowState(const XO_ServerState *state)
{
	u8 r, c, idx = 0;

	if (state == 0 || (!state->Alive && state->Status != XO_STATUS_ENDED)) {
		return;
	}

	if (!G_u8BoardDrawn || state->Turn != G_xLastTurn) {
		if (!G_u8BoardDrawn) {
			HTFT_vFillBackgroundColor(TFT_BLACK);
			BOARD_vDrawGrid(&BOARD_XO_LAYOUT);
		}
		G_xLastTurn = state->Turn;

		for (r = 0; r < 3; r++) {
			for (c = 0; c < 3; c++) {
				char ch = state->Board[idx++];
				XO_Mark mark = (ch == 'X') ? XO_MARK_X :
				               (ch == 'O') ? XO_MARK_O : XO_MARK_NONE;
				DrawCell(r, c, mark);
			}
		}

		if (state->Winner != XO_WINNER_NONE) {
			DrawWinLine(state->Winner);
		}
		G_u8BoardDrawn = 1;
	}
}

void XO_Animation_vShowIdle(void)
{
	HTFT_vFillBackgroundColor(TFT_BLACK);
	MENU_vText(30, 70, "XO: WAITING", TFT_CYAN, TFT_BLACK, 1);
}