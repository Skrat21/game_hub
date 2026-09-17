/*
 * Yatzy_Animation_prg.c
 *
 * Online Yatzy TFT renderer using BOARD_ANIM.
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

#include "Yatzy_Animation.h"

static u8 G_u8BoardDrawn = 0;
static u8 G_u8LastRoll = 0;

static void DrawDie(u8 idx, u8 value)
{
	u8 row = 0;
	u16 x = BOARD_YATZY_LAYOUT.X + idx * BOARD_YATZY_LAYOUT.CellW;
	u16 y = BOARD_YATZY_LAYOUT.Y;

	HTFT_vSetXPos(x, x + BOARD_YATZY_LAYOUT.MarkW - 1);
	HTFT_vSetYPos(y, y + BOARD_YATZY_LAYOUT.MarkH - 1);
	HTFT_vFillRectangle(TFT_WHITE);  /* die background */

	/* Draw pip pattern based on value (standard die face) */
	#define DIE_PIP(x,y) HTFT_vDrawPixel(x,y,TFT_BLACK)
	switch (value) {
		case 1:
			DIE_PIP(x + 11, y + 11);
			break;
		case 2:
			DIE_PIP(x + 4, y + 4);
			DIE_PIP(x + 18, y + 18);
			break;
		case 3:
			DIE_PIP(x + 4, y + 4);
			DIE_PIP(x + 11, y + 11);
			DIE_PIP(x + 18, y + 18);
			break;
		case 4:
			DIE_PIP(x + 4, y + 4);
			DIE_PIP(x + 18, y + 4);
			DIE_PIP(x + 4, y + 18);
			DIE_PIP(x + 18, y + 18);
			break;
		case 5:
			DIE_PIP(x + 4, y + 4);
			DIE_PIP(x + 18, y + 4);
			DIE_PIP(x + 11, y + 11);
			DIE_PIP(x + 4, y + 18);
			DIE_PIP(x + 18, y + 18);
			break;
		case 6:
			DIE_PIP(x + 4, y + 4);
			DIE_PIP(x + 11, y + 4);
			DIE_PIP(x + 18, y + 4);
			DIE_PIP(x + 4, y + 18);
			DIE_PIP(x + 11, y + 18);
			DIE_PIP(x + 18, y + 18);
			break;
	}
	#undef DIE_PIP
}

static void DrawScoreRow(const Yatzy_ServerState *state)
{
	char p1[6], p2[6];
	u8 len;

	/* P1: XX */
	p1[0] = '0' + (state->P1Score / 10);
	p1[1] = '0' + (state->P1Score % 10);
	p1[2] = '\0';
	MENU_vText(4, 142, "P1:", TFT_CYAN, TFT_BLACK, 1);
	MENU_vText(20, 142, p1, TFT_WHITE, TFT_BLACK, 1);

	/* P2: XX */
	p2[0] = '0' + (state->P2Score / 10);
	p2[1] = '0' + (state->P2Score % 10);
	p2[2] = '\0';
	MENU_vText(64, 142, "P2:", TFT_MAGENTA, TFT_BLACK, 1);
	MENU_vText(80, 142, p2, TFT_WHITE, TFT_BLACK, 1);
}

void Yatzy_Animation_vStart(void)
{
	G_u8BoardDrawn = 0;
	G_u8LastRoll = 0;
	HTFT_vFillBackgroundColor(TFT_BLACK);
	BOARD_vDrawGrid(&BOARD_YATZY_LAYOUT);
	LOADING_ANIM_vPlay();
}

void Yatzy_Animation_vShowState(const Yatzy_ServerState *state)
{
	u8 rollChanged = 0;

	if (state == 0 || (!state->Alive && state->Status != YATZY_STATUS_ENDED)) {
		return;
	}

	if (!G_u8BoardDrawn || state->RollsLeft != G_u8LastRoll) {
		if (!G_u8BoardDrawn) {
			HTFT_vFillBackgroundColor(TFT_BLACK);
			BOARD_vDrawGrid(&BOARD_YATZY_LAYOUT);
			DrawScoreRow(state);
		}

		G_u8LastRoll = state->RollsLeft;

		/* Draw dice - show all 5 dice */
		for (u8 i = 0; i < YATZY_DICE_COUNT; i++) {
			u8 value = state->Dice[i];
			if (value == 0) value = 1;  /* placeholder if not rolled yet */
			DrawDie(i, value);
		}

		G_u8BoardDrawn = 1;
	}
}

void Yatzy_Animation_vShowIdle(void)
{
	HTFT_vFillBackgroundColor(TFT_BLACK);
	BOARD_vDrawGrid(&BOARD_YATZY_LAYOUT);
	MENU_vText(20, 5, "YATZY", TFT_CYAN, TFT_BLACK, 1);
	MENU_vText(18, 142, "WAITING", TFT_GRAY, TFT_BLACK, 1);
}