/*
 * Memory_Animation_prg.c
 *
 * Online Memory matching game TFT renderer using BOARD_ANIM.
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#include "../../../LIB/STD_TYPES.h"
#include "../../../HAL/TFT/TFT_int.h"
#include "../../../APP/Common/BOARD_ANIM.h"
#include "../../../APP/Common/FONT3x5.h"
#include "../../../APP/Common/LOADING_ANIM.h"
#include "../../Menu/MENU_draw.h"

#include "Memory_Animation.h"

static u8 G_u8BoardDrawn = 0;
static u8 G_u8LastFlip1 = 0xFF, G_u8LastFlip2 = 0xFF;
static u8 G_u8LastMatchedPairs = 0;

static char SymbolToChar(Memory_Symbol sym)
{
	switch (sym) {
		case MEMORY_SYMBOL_BALL:  return 'B';
		case MEMORY_SYMBOL_PAPER: return 'P';
		case MEMORY_SYMBOL_DICE:  return 'D';
		case MEMORY_SYMBOL_FISH:  return 'F';
		default: return '-';
	}
}

static Memory_Symbol CharToSymbol(char ch)
{
	switch (ch) {
		case 'B': return MEMORY_SYMBOL_BALL;
		case 'P': return MEMORY_SYMBOL_PAPER;
		case 'D': return MEMORY_SYMBOL_DICE;
		case 'F': return MEMORY_SYMBOL_FISH;
		default: return MEMORY_SYMBOL_NONE;
	}
}

static void DrawCard(u8 idx, char ch, u8 revealed)
{
	u8 row = idx / 4;   /* 4 columns, 2 rows */
	u8 col = idx % 4;

	if (revealed || ch != '-') {
		/* Face up: show symbol */
		Memory_Symbol sym = CharToSymbol(ch);
		switch (sym) {
			case MEMORY_SYMBOL_BALL:
				BOARD_vMark(&BOARD_MEMORY_LAYOUT, row, col, 'B', TFT_RED);
				break;
			case MEMORY_SYMBOL_PAPER:
				BOARD_vMark(&BOARD_MEMORY_LAYOUT, row, col, 'P', TFT_GREEN);
				break;
			case MEMORY_SYMBOL_DICE:
				BOARD_vMark(&BOARD_MEMORY_LAYOUT, row, col, 'D', TFT_YELLOW);
				break;
			case MEMORY_SYMBOL_FISH:
				BOARD_vMark(&BOARD_MEMORY_LAYOUT, row, col, 'F', TFT_BLUE);
				break;
			default:
				BOARD_vCell(&BOARD_MEMORY_LAYOUT, row, col, TFT_BLACK);
		}
	} else {
		/* Face down: show back pattern */
		BOARD_vCell(&BOARD_MEMORY_LAYOUT, row, col, TFT_GRAY);
	}
}

void Memory_Animation_vStart(void)
{
	G_u8BoardDrawn = 0;
	G_u8LastFlip1 = G_u8LastFlip2 = 0xFF;
	G_u8LastMatchedPairs = 0;
	HTFT_vFillBackgroundColor(TFT_BLACK);
	BOARD_vDrawGrid(&BOARD_MEMORY_LAYOUT);
	LOADING_ANIM_vPlay();
}

void Memory_Animation_vShowState(const Memory_ServerState *state)
{
	if (state == 0 || (!state->Alive && state->Status != MEMORY_STATUS_ENDED)) {
		return;
	}

	if (!G_u8BoardDrawn || state->Flip1 != G_u8LastFlip1
			|| state->Flip2 != G_u8LastFlip2
			|| state->MatchedPairs != G_u8LastMatchedPairs) {
		if (G_u8LastMatchedPairs == 4 && state->MatchedPairs == 4) {
			/* All pairs matched - brief celebration */
			HTFT_vFillBackgroundColor(TFT_BLACK);
			BOARD_vDrawGrid(&BOARD_MEMORY_LAYOUT);
		} else if (!G_u8BoardDrawn) {
			HTFT_vFillBackgroundColor(TFT_BLACK);
			BOARD_vDrawGrid(&BOARD_MEMORY_LAYOUT);
		}

		for (u8 i = 0; i < MEMORY_CELL_COUNT; i++) {
			u8 revealed = (state->Board[i] != '-');
			DrawCard(i, state->Board[i], revealed);
		}

		G_u8BoardDrawn = 1;
		G_u8LastFlip1 = state->Flip1;
		G_u8LastFlip2 = state->Flip2;
		G_u8LastMatchedPairs = state->MatchedPairs;
	}
}

void Memory_Animation_vShowIdle(void)
{
	HTFT_vFillBackgroundColor(TFT_BLACK);
	BOARD_vDrawGrid(&BOARD_MEMORY_LAYOUT);
	MENU_vText(23, 130, "MEMORY", TFT_CYAN, TFT_BLACK, 1);
	MENU_vText(20, 143, "WAITING", TFT_GRAY, TFT_BLACK, 1);
}