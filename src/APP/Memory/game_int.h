/*
 * game_int.h
 *
 * Online Memory matching game — server state model.
 *
 * Protocol (website/Projecth/memory_backend.php, status/memory.txt):
 *   state=ALIVE;game=memory;room=<ID>;event=<EVENT>;status=waiting|playing|result|ended;
 *   turn=p1|p2|;flip1=<idx>;flip2=<idx>;matched_pairs=<n>;board=<8 chars B/P/D/F|-;>;
 *   winner=p1|p2|draw|;round=<n>;time=..
 *
 * `board` is 8 characters: each card shows '-' (face down), or the symbol
 * 'B' (Ball), 'P' (Paper), 'D' (Dice), 'F' (Fish) for face-up matched cards.
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_MEMORY_GAME_INT_H_
#define APP_MEMORY_GAME_INT_H_

#include "../../LIB/STD_TYPES.h"

#define MEMORY_CELL_COUNT 8

typedef enum {
	MEMORY_STATUS_WAITING,
	MEMORY_STATUS_PLAYING,
	MEMORY_STATUS_RESULT,
	MEMORY_STATUS_ENDED
} Memory_Status;

typedef enum {
	MEMORY_SYMBOL_NONE,
	MEMORY_SYMBOL_BALL,    /* B */
	MEMORY_SYMBOL_PAPER,   /* P */
	MEMORY_SYMBOL_DICE,    /* D */
	MEMORY_SYMBOL_FISH     /* F */
} Memory_Symbol;

typedef enum {
	MEMORY_WINNER_NONE,
	MEMORY_WINNER_P1,
	MEMORY_WINNER_P2,
	MEMORY_WINNER_DRAW
} Memory_Winner;

typedef struct {
	u8 Alive;
	Memory_Status Status;
	u8 Turn;               /* 1 or 2 */
	u8 Flip1;              /* -1 or card index 0-7 */
	u8 Flip2;              /* -1 or card index 0-7 */
	u8 MatchedPairs;       /* 0-4 */
	char Board[MEMORY_CELL_COUNT + 1];  /* 'B','P','D','F','-' */
	Memory_Winner Winner;
	u8 Round;
	char Event[20];
	char Room[12];
} Memory_ServerState;

void Memory_vStart(void);
void Memory_vUpdate(const Memory_ServerState *state);
void Memory_vStop(void);

#endif /* APP_MEMORY_GAME_INT_H_ */