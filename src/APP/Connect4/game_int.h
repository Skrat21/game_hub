/*
 * game_int.h
 *
 * Online Connect 4 — server state model.
 *
 * Protocol (website/Projecth/connect4_backend.php, status/connect4.txt):
 *   state=ALIVE;game=connect4;room=<ID>;event=<EVENT>;last_player=p1|p2|;
 *   last_move_row=<row>;last_move_col=<col>;board=<42 chars R/Y/-|>;
 *   turn=p1|p2|;winner=p1|p2|draw|;round=<n>;time=..
 *
 * `board` is 42 chars row-major (6x7, row 0 = top): 'R'/'Y'/'-' (Red/Yellow/empty).
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_CONNECT4_GAME_INT_H_
#define APP_CONNECT4_GAME_INT_H_

#include "../../LIB/STD_TYPES.h"

/* Connect 4 board constants */
#define CONNECT4_ROWS 6
#define CONNECT4_COLS 7
#define CONNECT4_CELL_COUNT (CONNECT4_ROWS * CONNECT4_COLS)

typedef enum {
	CONNECT4_STATUS_WAITING,
	CONNECT4_STATUS_PLAYING,
	CONNECT4_STATUS_RESULT,
	CONNECT4_STATUS_ENDED
} Connect4_Status;

typedef enum {
	CONNECT4_WINNER_NONE,
	CONNECT4_WINNER_P1,
	CONNECT4_WINNER_P2,
	CONNECT4_WINNER_DRAW
} Connect4_Winner;

typedef enum {
	CONNECT4_MARK_NONE,
	CONNECT4_MARK_RED,
	CONNECT4_MARK_YELLOW
} Connect4_Mark;

typedef struct {
	u8 Alive;
	Connect4_Status Status;
	char Room[12];
	char LastPlayer[4];
	u8 LastMoveRow;
	u8 LastMoveCol;
	char Board[CONNECT4_CELL_COUNT + 1];  /* 'R', 'Y' or '-' */
	Connect4_Mark Turn;
	Connect4_Winner Winner;
	u8 Round;
	char Event[20];
} Connect4_ServerState;

void Connect4_vStart(void);
void Connect4_vUpdate(const Connect4_ServerState *state);
void Connect4_vStop(void);

#endif /* APP_CONNECT4_GAME_INT_H_ */