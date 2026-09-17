/*
 * BOARD_ANIM.c
 *
 * Shared board/tile renderer (see BOARD_ANIM.h).
 *
 * All geometry is reproduced from the original offline renderers:
 *   TICTACTOE.c : MENU_vGrid(14, 30, 100, 100, 3, 3)  + Cursor()/Cell()
 *   CONNECT4.c  : MENU_vGrid(15, 26,  98, 108, 7, 6)  + Cursor()
 * so the offline games keep their exact pixels after being rewired to this
 * module, and the online games render identically for free.
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#include "../../LIB/STD_TYPES.h"
#include "../../HAL/TFT/TFT_int.h"
#include "../Menu/MENU_draw.h"

#include "BOARD_ANIM.h"

/* 3x3, grid 100x100 at (14, 30): pitch 33, marker 30x30 inset 2,
 * 2x-scaled X/O glyph drawn at inset (11, 8). Matches offline XO. */
const BOARD_Layout_t BOARD_XO_LAYOUT = {
	.X = 14, .Y = 30,
	.GridW = 100, .GridH = 100,
	.Rows = 3, .Cols = 3,
	.CellW = 33, .CellH = 33,
	.MarkInsetX = 2, .MarkInsetY = 2,
	.MarkW = 30, .MarkH = 30,
	.TextInsetX = 11, .TextInsetY = 8, .TextScale = 2,
	.ColCursorDY = 0, .ColCursorInsetX = 0, .ColCursorW = 0, .ColCursorH = 0
};

/* 7 cols x 6 rows, grid 98x108 at (15, 26): pitch 14x18, coin 10x14 inset 2,
 * column cursor 9x5 drawn 8 px above the grid at inset 2. Offline C4. */
const BOARD_Layout_t BOARD_CONNECT4_LAYOUT = {
	.X = 15, .Y = 26,
	.GridW = 98, .GridH = 108,
	.Rows = 6, .Cols = 7,
	.CellW = 14, .CellH = 18,
	.MarkInsetX = 2, .MarkInsetY = 2,
	.MarkW = 10, .MarkH = 14,
	.TextInsetX = 0, .TextInsetY = 0, .TextScale = 0,
	.ColCursorDY = 8, .ColCursorInsetX = 2, .ColCursorW = 9, .ColCursorH = 5
};

/* 4x2 memory tile board: 108x68 at (10, 36), tiles 25x32 inset 1. */
const BOARD_Layout_t BOARD_MEMORY_LAYOUT = {
	.X = 10, .Y = 36,
	.GridW = 108, .GridH = 68,
	.Rows = 2, .Cols = 4,
	.CellW = 27, .CellH = 34,
	.MarkInsetX = 1, .MarkInsetY = 1,
	.MarkW = 25, .MarkH = 32,
	.TextInsetX = 0, .TextInsetY = 0, .TextScale = 0,
	.ColCursorDY = 0, .ColCursorInsetX = 0, .ColCursorW = 0, .ColCursorH = 0
};

/* 5 dice in a row: pitch 24 at (4, 64), die face 22x22 inset 1. */
const BOARD_Layout_t BOARD_YATZY_LAYOUT = {
	.X = 4, .Y = 64,
	.GridW = 116, .GridH = 24,
	.Rows = 1, .Cols = 5,
	.CellW = 24, .CellH = 24,
	.MarkInsetX = 1, .MarkInsetY = 1,
	.MarkW = 22, .MarkH = 22,
	.TextInsetX = 0, .TextInsetY = 0, .TextScale = 0,
	.ColCursorDY = 0, .ColCursorInsetX = 0, .ColCursorW = 0, .ColCursorH = 0
};

static u16 CellX(const BOARD_Layout_t *layout, u8 col)
{
	return (u16)(layout->X + (u16)col * layout->CellW);
}

static u16 CellY(const BOARD_Layout_t *layout, u8 row)
{
	return (u16)(layout->Y + (u16)row * layout->CellH);
}

void BOARD_vDrawGrid(const BOARD_Layout_t *layout)
{
	if (layout == 0) return;
	MENU_vGrid(layout->X, layout->Y, layout->GridW, layout->GridH,
	           layout->Cols, layout->Rows);
}

void BOARD_vCell(const BOARD_Layout_t *layout, u8 row, u8 col, u16 color)
{
	if (layout == 0 || row >= layout->Rows || col >= layout->Cols) return;
	MENU_vRect((u16)(CellX(layout, col) + layout->MarkInsetX),
	           (u16)(CellY(layout, row) + layout->MarkInsetY),
	           layout->MarkW, layout->MarkH, color);
}

void BOARD_vCursor(const BOARD_Layout_t *layout, u8 row, u8 col, u16 color)
{
	u16 x, y, w, h;
	if (layout == 0 || row >= layout->Rows || col >= layout->Cols) return;

	/* Original: x+1 / y+1 with a 32x32 outline for a 30x30 marker. */
	x = (u16)(CellX(layout, col) + layout->MarkInsetX - 1);
	y = (u16)(CellY(layout, row) + layout->MarkInsetY - 1);
	w = (u16)(layout->MarkW + 2);
	h = (u16)(layout->MarkH + 2);

	MENU_vRect(x, y, w, 1, color);
	MENU_vRect(x, (u16)(y + h - 1), w, 1, color);
	MENU_vRect(x, y, 1, h, color);
	MENU_vRect((u16)(x + w - 1), y, 1, h, color);
}

void BOARD_vColumnCursor(const BOARD_Layout_t *layout, u8 col, u16 color)
{
	if (layout == 0 || col >= layout->Cols) return;
	if (layout->ColCursorDY > layout->Y) return;

	MENU_vRect((u16)(CellX(layout, col) + layout->ColCursorInsetX),
	           (u16)(layout->Y - layout->ColCursorDY),
	           layout->ColCursorW, layout->ColCursorH, color);
}

void BOARD_vToken(const BOARD_Layout_t *layout, u8 row, u8 col, u16 color)
{
	BOARD_vCell(layout, row, col, color);
}

void BOARD_vMark(const BOARD_Layout_t *layout, u8 row, u8 col, char mark,
                 u16 color)
{
	char text[2];
	if (layout == 0 || layout->TextScale == 0) return;

	BOARD_vCell(layout, row, col, TFT_BLACK);

	text[0] = mark;
	text[1] = '\0';
	MENU_vText((u16)(CellX(layout, col) + layout->TextInsetX),
	           (u16)(CellY(layout, row) + layout->TextInsetY),
	           text, color, TFT_BLACK, layout->TextScale);
}

void BOARD_vWinLine(const BOARD_Layout_t *layout,
                    u8 rowA, u8 colA, u8 rowB, u8 colB, u16 color)
{
	u16 x0, y0, x1, y1;
	if (layout == 0) return;

	BOARD_vCellCentre(layout, rowA, colA, &x0, &y0);
	BOARD_vCellCentre(layout, rowB, colB, &x1, &y1);
	HTFT_vDrawLine(x0, y0, x1, y1, color);
}

void BOARD_vCellCentre(const BOARD_Layout_t *layout, u8 row, u8 col,
                       u16 *x, u16 *y)
{
	if (layout == 0) return;
	if (x != 0) *x = (u16)(CellX(layout, col) + layout->CellW / 2);
	if (y != 0) *y = (u16)(CellY(layout, row) + layout->CellH / 2);
}
