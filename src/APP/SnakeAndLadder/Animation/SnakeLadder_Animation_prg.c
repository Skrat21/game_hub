/*
 * SnakeLadder_Animation_prg.c
 *
 * Snake and Ladder TFT renderer and animations.
 *
 *  Created on: Sep 16, 2026
 *      Author: AHMED SHERIF
 */

#include "../../../LIB/STD_TYPES.h"
#include "../../../HAL/TFT/TFT_int.h"
#include "../../../MCAL/SYSTICK/SYSTICK_int.h"
#include "../../../APP/Common/FONT3x5.h"
#include "../../../APP/Common/LOADING_ANIM.h"

#include "SnakeLadder_Animation.h"

typedef struct {
	u8 start;
	u8 end;
} BoardFeature_t;

static const BoardFeature_t G_xLadders[8] = {
	{ 3, 22 },
	{ 8, 30 },
	{ 15, 44 },
	{ 28, 55 },
	{ 36, 57 },
	{ 51, 72 },
	{ 62, 81 },
	{ 71, 91 }
};

static const BoardFeature_t G_xSnakes[8] = {
	{ 27, 5 },
	{ 39, 17 },
	{ 48, 26 },
	{ 58, 37 },
	{ 66, 45 },
	{ 79, 60 },
	{ 89, 53 },
	{ 99, 41 }
};

static u8 G_u8BoardDrawn = 0;
static u8 G_u8LastP1Pos = 0;
static u8 G_u8LastP2Pos = 0;
static u8 G_u8LastRound = 0;

static u8 StringsEqual(const char *left, const char *right)
{
	while (*left != '\0' && *right != '\0' && *left == *right) {
		left++;
		right++;
	}
	return *left == '\0' && *right == '\0';
}

static void AppendNumber(char *text, u8 *len, u8 val)
{
	if (val >= 100) {
		text[(*len)++] = '0' + (val / 100);
	}
	if (val >= 10) {
		text[(*len)++] = '0' + ((val / 10) % 10);
	}
	text[(*len)++] = '0' + (val % 10);
}

static void GetSquareBounds(u8 sq, u16 *x0, u16 *y0, u16 *x1, u16 *y1)
{
	if (sq < 1) sq = 1;
	if (sq > 100) sq = 100;

	u8 rowFromBottom = (sq - 1) / 10;
	u8 indexInRow = (sq - 1) % 10;
	u8 col = (rowFromBottom % 2 == 0) ? indexInRow : (9 - indexInRow);
	u8 rowFromTop = 9 - rowFromBottom;

	*x0 = 4 + col * 12;
	*y0 = 18 + rowFromTop * 12;
	*x1 = *x0 + 11;
	*y1 = *y0 + 11;
}

static void GetSquareCenter(u8 sq, u16 *cx, u16 *cy)
{
	u16 x0, y0, x1, y1;
	GetSquareBounds(sq, &x0, &y0, &x1, &y1);
	*cx = x0 + 6;
	*cy = y0 + 6;
}

static u16 GetTileColor(u8 sq)
{
	u8 rowFromBottom = (sq - 1) / 10;
	u8 indexInRow = (sq - 1) % 10;
	u8 col = (rowFromBottom % 2 == 0) ? indexInRow : (9 - indexInRow);
	u8 rowFromTop = 9 - rowFromBottom;

	if ((col + rowFromTop) % 2 == 0) {
		return 0x2104; /* Dark slate */
	} else {
		return 0x1082; /* Deep charcoal */
	}
}

static void DrawTile(u8 sq)
{
	u16 x0, y0, x1, y1;
	GetSquareBounds(sq, &x0, &y0, &x1, &y1);

	HTFT_vSetXPos(x0, x1);
	HTFT_vSetYPos(y0, y1);
	HTFT_vFillRectangle(GetTileColor(sq));

	/* Tile 1 start accent */
	if (sq == 1) {
		HTFT_vDrawPixel(x0 + 1, y0 + 1, TFT_GREEN);
		HTFT_vDrawPixel(x0 + 2, y0 + 1, TFT_GREEN);
		HTFT_vDrawPixel(x0 + 1, y0 + 2, TFT_GREEN);
	}
	/* Tile 100 goal accent */
	else if (sq == 100) {
		HTFT_vDrawPixel(x1 - 1, y0 + 1, TFT_YELLOW);
		HTFT_vDrawPixel(x1 - 2, y0 + 1, TFT_YELLOW);
		HTFT_vDrawPixel(x1 - 1, y0 + 2, TFT_YELLOW);
	}
}

static void DrawLadder(const BoardFeature_t *ladder, u16 color)
{
	u16 x0, y0, x1, y1;
	GetSquareCenter(ladder->start, &x0, &y0);
	GetSquareCenter(ladder->end, &x1, &y1);

	HTFT_vDrawLine(x0 - 1, y0, x1 - 1, y1, color);
	HTFT_vDrawLine(x0 + 1, y0, x1 + 1, y1, color);

	for (u8 i = 1; i <= 3; i++) {
		u16 rx = x0 + (u16)(((i * (s16)(x1 - x0))) >> 2);
		u16 ry = y0 + (u16)(((i * (s16)(y1 - y0))) >> 2);
		HTFT_vDrawLine(rx - 2, ry, rx + 2, ry, TFT_YELLOW);
	}
}

static void DrawSnake(const BoardFeature_t *snake, u16 color)
{
	u16 x0, y0, x1, y1;
	GetSquareCenter(snake->start, &x0, &y0);
	GetSquareCenter(snake->end, &x1, &y1);

	HTFT_vDrawLine(x0, y0, x1, y1, color);
	HTFT_vFillCircle(x0, y0, 2, TFT_LIME);
	HTFT_vDrawCircle(x0, y0, 2, color);
	HTFT_vDrawPixel(x1, y1, color);
}

static void DrawAllFeatures(void)
{
	for (u8 i = 0; i < 8; i++) {
		DrawLadder(&G_xLadders[i], TFT_ORANGE);
	}
	for (u8 i = 0; i < 8; i++) {
		DrawSnake(&G_xSnakes[i], TFT_GREEN);
	}
}

static void RedrawTileFeatures(u8 sq)
{
	(void)sq;
	/* DrawTile() erases the entire 12x12 tile square, including any
	 * snake/ladder line segments that merely pass through it.  Because
	 * DrawLadder/DrawSnake draw full lines between distant tile centres,
	 * those lines frequently cross tiles that do not host either endpoint.
	 * To keep the board intact after a partial clear we therefore redraw
	 * every snake and ladder feature, not just the ones anchored on this
	 * particular tile. */
	for (u8 i = 0; i < 8; i++) {
		DrawLadder(&G_xLadders[i], TFT_ORANGE);
	}
	for (u8 i = 0; i < 8; i++) {
		DrawSnake(&G_xSnakes[i], TFT_GREEN);
	}
}

static void DrawTokenP1(u8 pos)
{
	if (pos >= 1 && pos <= 100) {
		u16 cx, cy;
		GetSquareCenter(pos, &cx, &cy);
		HTFT_vFillCircle(cx - 2, cy, 2, TFT_CYAN);
		HTFT_vDrawPixel(cx - 2, cy, TFT_WHITE);
	}
}

static void DrawTokenP2(u8 pos)
{
	if (pos >= 1 && pos <= 100) {
		u16 cx, cy;
		GetSquareCenter(pos, &cx, &cy);
		HTFT_vFillCircle(cx + 2, cy, 2, TFT_YELLOW);
		HTFT_vDrawPixel(cx + 2, cy, TFT_RED);
	}
}

static void ClearSquare(u8 sq, u8 other_p_pos, u8 is_p1)
{
	if (sq >= 1 && sq <= 100) {
		DrawTile(sq);
		RedrawTileFeatures(sq);
		if (other_p_pos == sq) {
			if (is_p1) {
				DrawTokenP2(other_p_pos);
			} else {
				DrawTokenP1(other_p_pos);
			}
		}
	}
}

static void DrawFullBoard(u8 p1_pos, u8 p2_pos)
{
	HTFT_vSetXPos(3, 124);
	HTFT_vSetYPos(17, 138);
	HTFT_vFillRectangle(TFT_DARK_GRAY);

	for (u8 sq = 1; sq <= 100; sq++) {
		DrawTile(sq);
	}

	DrawAllFeatures();

	if (p1_pos > 0) {
		DrawTokenP1(p1_pos);
	}
	if (p2_pos > 0) {
		DrawTokenP2(p2_pos);
	}
}

static void DrawHeader(const SnakeLadder_ServerState *state)
{
	HTFT_vSetXPos(0, 127);
	HTFT_vSetYPos(0, 16);
	HTFT_vFillRectangle(TFT_BLACK);

	if (state->Status == SL_STATUS_WAITING) {
		FONT3x5_vDrawText("WAITING FOR P2", 4, 4, TFT_YELLOW);
	} else if (state->Status == SL_STATUS_RESULT) {
		if (state->Winner == SL_WINNER_P1) {
			FONT3x5_vDrawText("P1 WINS MATCH!", 4, 4, TFT_CYAN);
		} else if (state->Winner == SL_WINNER_P2) {
			FONT3x5_vDrawText("P2 WINS MATCH!", 4, 4, TFT_YELLOW);
		} else {
			FONT3x5_vDrawText("MATCH FINISHED", 4, 4, TFT_WHITE);
		}
	} else if (state->Status == SL_STATUS_ENDED) {
		FONT3x5_vDrawText("MATCH ENDED", 4, 4, TFT_RED);
	} else {
		if (state->Turn == SL_TURN_P1) {
			FONT3x5_vDrawText("P1 TURN", 4, 4, TFT_CYAN);
		} else if (state->Turn == SL_TURN_P2) {
			FONT3x5_vDrawText("P2 TURN", 4, 4, TFT_YELLOW);
		} else {
			FONT3x5_vDrawText("PLAYING", 4, 4, TFT_WHITE);
		}
	}

	FONT3x5_vDrawText("DICE:", 84, 4, TFT_LIGHT_GRAY);
	if (state->LastRoll >= 1 && state->LastRoll <= 6) {
		char rollStr[2];
		rollStr[0] = '0' + state->LastRoll;
		rollStr[1] = '\0';
		FONT3x5_vDrawText(rollStr, 108, 4, TFT_GREEN);
	} else {
		FONT3x5_vDrawText("-", 108, 4, TFT_GRAY);
	}

	HTFT_vSetXPos(0, 127);
	HTFT_vSetYPos(16, 16);
	HTFT_vFillRectangle(TFT_DARK_GRAY);
}

static void DrawFooter(const SnakeLadder_ServerState *state)
{
	HTFT_vSetXPos(0, 127);
	HTFT_vSetYPos(139, 159);
	HTFT_vFillRectangle(TFT_BLACK);

	HTFT_vSetXPos(0, 127);
	HTFT_vSetYPos(139, 139);
	HTFT_vFillRectangle(TFT_DARK_GRAY);

	char p1Text[12] = "P1: ";
	u8 p1Len = 4;
	if (state->Player1_Pos == 0) {
		p1Text[p1Len++] = 'S';
		p1Text[p1Len++] = 'T';
		p1Text[p1Len++] = 'R';
		p1Text[p1Len++] = 'T';
		p1Text[p1Len] = '\0';
	} else {
		AppendNumber(p1Text, &p1Len, state->Player1_Pos);
		p1Text[p1Len] = '\0';
	}
	FONT3x5_vDrawText(p1Text, 4, 142, TFT_CYAN);

	char p2Text[12] = "P2: ";
	u8 p2Len = 4;
	if (state->Player2_Pos == 0) {
		p2Text[p2Len++] = 'S';
		p2Text[p2Len++] = 'T';
		p2Text[p2Len++] = 'R';
		p2Text[p2Len++] = 'T';
		p2Text[p2Len] = '\0';
	} else {
		AppendNumber(p2Text, &p2Len, state->Player2_Pos);
		p2Text[p2Len] = '\0';
	}
	FONT3x5_vDrawText(p2Text, 72, 142, TFT_YELLOW);

	if (StringsEqual(state->Event, "ladder_climb")) {
		FONT3x5_vDrawText(">> LADDER CLIMB! <<", 16, 151, TFT_YELLOW);
	} else if (StringsEqual(state->Event, "snake_slide")) {
		FONT3x5_vDrawText("!! SNAKE SLIDE !!", 20, 151, TFT_RED);
	} else if (StringsEqual(state->Event, "game_won")) {
		FONT3x5_vDrawText("*** REACHED 100 ***", 16, 151, TFT_GREEN);
	} else if (StringsEqual(state->Event, "dice_roll")) {
		FONT3x5_vDrawText("DICE ROLLED", 40, 151, TFT_LIGHT_GRAY);
	} else if (StringsEqual(state->Event, "create_room")) {
		FONT3x5_vDrawText("ROOM CREATED", 36, 151, TFT_GRAY);
	} else if (StringsEqual(state->Event, "player_joined")) {
		FONT3x5_vDrawText("PLAYER 2 JOINED", 30, 151, TFT_GREEN);
	} else if (state->Round > 0) {
		char rndText[16] = "ROUND ";
		u8 rndLen = 6;
		AppendNumber(rndText, &rndLen, state->Round);
		rndText[rndLen] = '\0';
		FONT3x5_vDrawText(rndText, 44, 151, TFT_GRAY);
	}
}

static void AnimateSpecialEvent(const SnakeLadder_ServerState *state)
{
	if (StringsEqual(state->Event, "ladder_climb")) {
		for (u8 i = 0; i < 8; i++) {
			if (G_xLadders[i].end == state->Player1_Pos || G_xLadders[i].end == state->Player2_Pos) {
				DrawLadder(&G_xLadders[i], TFT_WHITE);
				MSYSTICK_vSetDelay_ms(60);
				DrawLadder(&G_xLadders[i], TFT_ORANGE);
				break;
			}
		}
	} else if (StringsEqual(state->Event, "snake_slide")) {
		for (u8 i = 0; i < 8; i++) {
			if (G_xSnakes[i].end == state->Player1_Pos || G_xSnakes[i].end == state->Player2_Pos) {
				DrawSnake(&G_xSnakes[i], TFT_RED);
				MSYSTICK_vSetDelay_ms(60);
				DrawSnake(&G_xSnakes[i], TFT_GREEN);
				break;
			}
		}
	}
}

void SnakeLadder_Animation_vStart(void)
{
	HTFT_vFillBackgroundColor(TFT_BLACK);
	LOADING_ANIM_vPlay();
	G_u8BoardDrawn = 0;
	G_u8LastP1Pos = 0;
	G_u8LastP2Pos = 0;
	G_u8LastRound = 0;
}

void SnakeLadder_Animation_vShowState(const SnakeLadder_ServerState *state)
{
	if (state == 0) {
		return;
	}

	if (!G_u8BoardDrawn || state->Round != G_u8LastRound || state->Status == SL_STATUS_WAITING) {
		HTFT_vFillBackgroundColor(TFT_BLACK);
		DrawFullBoard(state->Player1_Pos, state->Player2_Pos);
		G_u8BoardDrawn = 1;
		G_u8LastP1Pos = state->Player1_Pos;
		G_u8LastP2Pos = state->Player2_Pos;
		G_u8LastRound = state->Round;
	} else {
		/* Targeted partial updates to avoid board flicker */
		if (state->Player1_Pos != G_u8LastP1Pos) {
			ClearSquare(G_u8LastP1Pos, G_u8LastP2Pos, 1);
			G_u8LastP1Pos = state->Player1_Pos;
			DrawTokenP1(state->Player1_Pos);
		}
		if (state->Player2_Pos != G_u8LastP2Pos) {
			ClearSquare(G_u8LastP2Pos, G_u8LastP1Pos, 0);
			G_u8LastP2Pos = state->Player2_Pos;
			DrawTokenP2(state->Player2_Pos);
		}
	}

	AnimateSpecialEvent(state);
	DrawHeader(state);
	DrawFooter(state);
}

void SnakeLadder_Animation_vShowIdle(void)
{
	HTFT_vFillBackgroundColor(TFT_BLACK);
	FONT3x5_vDrawText("SNAKE AND LADDER", 24, 70, TFT_CYAN);
	FONT3x5_vDrawText("WAITING FOR GAME", 24, 85, TFT_GRAY);
	G_u8BoardDrawn = 0;
	G_u8LastP1Pos = 0;
	G_u8LastP2Pos = 0;
	G_u8LastRound = 0;
}

