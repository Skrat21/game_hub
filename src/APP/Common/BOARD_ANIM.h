/*
 * BOARD_ANIM.h
 *
 * Shared board/tile renderer for every grid-based game (offline and online).
 *
 * This module is the single source of truth for "board drawing" concepts:
 * the grid itself, a cell highlight/cursor, a filled token, a text marker
 * (X/O) and a win-line highlight.
 *
 * It was extracted from the offline Tic-Tac-Toe and Connect 4 renderers so
 * that the online counterparts reuse the exact same drawing code instead of
 * duplicating it. Geometry is data-driven through BOARD_Layout_t, so any
 * board-based game (XO, Connect 4, Memory, Yatzy) routes through this one
 * implementation.
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_COMMON_BOARD_ANIM_H_
#define APP_COMMON_BOARD_ANIM_H_

#include "../../LIB/STD_TYPES.h"

/*
 * Board geometry descriptor. Cell (row, col) has its top-left corner at
 *   (X + col * CellW, Y + row * CellH)
 * which reproduces the offline renderers' addressing exactly.
 */
typedef struct {
	u16 X;              /* grid origin X (as passed to MENU_vGrid) */
	u16 Y;              /* grid origin Y (as passed to MENU_vGrid) */
	u16 GridW;          /* nominal grid width passed to MENU_vGrid */
	u16 GridH;          /* nominal grid height passed to MENU_vGrid */
	u8  Rows;           /* number of rows */
	u8  Cols;           /* number of columns */
	u16 CellW;          /* horizontal cell pitch (GridW / Cols) */
	u16 CellH;          /* vertical cell pitch (GridH / Rows) */
	u16 MarkInsetX;     /* token/marker rect X offset inside the cell */
	u16 MarkInsetY;     /* token/marker rect Y offset inside the cell */
	u16 MarkW;          /* token/marker rect width */
	u16 MarkH;          /* token/marker rect height */
	u16 TextInsetX;     /* text marker X offset inside the cell */
	u16 TextInsetY;     /* text marker Y offset inside the cell */
	u8  TextScale;      /* text marker scale (0 = text markers unused) */
	u16 ColCursorDY;    /* column-cursor distance ABOVE the grid origin */
	u16 ColCursorInsetX;/* column-cursor X offset inside the column */
	u16 ColCursorW;     /* column-cursor width */
	u16 ColCursorH;     /* column-cursor height */
} BOARD_Layout_t;

/* Ready-made layouts. Using these guarantees the offline and online versions
 * of a game are pixel-identical. */
extern const BOARD_Layout_t BOARD_XO_LAYOUT;       /* 3x3  @ 14,30  100x100 */
extern const BOARD_Layout_t BOARD_CONNECT4_LAYOUT; /* 7x6  @ 15,26   98x108 */
extern const BOARD_Layout_t BOARD_MEMORY_LAYOUT;   /* 4x2  tile board      */
extern const BOARD_Layout_t BOARD_YATZY_LAYOUT;    /* 5x1  dice row        */

/* Draws the empty grid (reuses MENU_vGrid, same as the offline games). */
void BOARD_vDrawGrid(const BOARD_Layout_t *layout);

/* Fills one cell's marker area with `color` (TFT_BLACK wipes it). */
void BOARD_vCell(const BOARD_Layout_t *layout, u8 row, u8 col, u16 color);

/* Rectangular cell outline cursor (offline Tic-Tac-Toe style). */
void BOARD_vCursor(const BOARD_Layout_t *layout, u8 row, u8 col, u16 color);

/* Small column cursor drawn above the grid (offline Connect 4 style). */
void BOARD_vColumnCursor(const BOARD_Layout_t *layout, u8 col, u16 color);

/* Clears the cell, then draws a filled token rectangle (Connect 4 coin). */
void BOARD_vToken(const BOARD_Layout_t *layout, u8 row, u8 col, u16 color);

/* Clears the cell, then draws a scaled text marker ('X', 'O', ...). */
void BOARD_vMark(const BOARD_Layout_t *layout, u8 row, u8 col, char mark,
                 u16 color);

/* Win highlight: line through the centres of two cells. This is genuinely
 * new capability (neither offline game had a win-line animation). */
void BOARD_vWinLine(const BOARD_Layout_t *layout,
                    u8 rowA, u8 colA, u8 rowB, u8 colB, u16 color);

/* Centre of a cell, used by modules that draw their own overlay text. */
void BOARD_vCellCentre(const BOARD_Layout_t *layout, u8 row, u8 col,
                       u16 *x, u16 *y);

#endif /* APP_COMMON_BOARD_ANIM_H_ */
