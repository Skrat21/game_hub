/*
 * game_int.h
 *
 * Online Tic-Tac-Toe (XO) — server state model.
 *
 * Protocol (website/Projecth/xo_backend.php, status/xo.txt):
 *   state=ALIVE;game=xo;room=<ID>;event=<EVENT>;status=waiting|playing|
 *   result|ended;board=<9 chars>;turn=X|O|;winner=X|O|draw|;round=<n>;time=..
 *
 * `board` is 9 characters in row-major order, each 'X', 'O' or '-'.
 * The firmware is a display mirror only (no local input), exactly like the
 * existing rps / mathquiz / snakeladder online modules.
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_TICTACTOE_GAME_INT_H_
#define APP_TICTACTOE_GAME_INT_H_

#include "../../LIB/STD_TYPES.h"

/* Number of board cells (3x3). */
#define XO_CELL_COUNT 9

typedef enum {
	XO_STATUS_WAITING,
	XO_STATUS_PLAYING,
	XO_STATUS_RESULT,
	XO_STATUS_ENDED
} XO_Status;

typedef enum {
	XO_MARK_NONE,
	XO_MARK_X,
	XO_MARK_O
} XO_Mark;

typedef enum {
	XO_WINNER_NONE,
	XO_WINNER_X,
	XO_WINNER_O,
	XO_WINNER_DRAW
} XO_Winner;

typedef struct {
	u8 Alive;
	XO_Status Status;
	u8 Round;
	char Board[XO_CELL_COUNT];   /* 'X', 'O' or '-' */
	XO_Mark Turn;
	XO_Winner Winner;
	char Event[20];
	char Room[12];
} XO_ServerState;

void XO_vStart(void);
void XO_vUpdate(const XO_ServerState *state);
void XO_vStop(void);

#endif /* APP_TICTACTOE_GAME_INT_H_ */
