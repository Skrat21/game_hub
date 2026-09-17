/*
 * game_int.h
 *
 * Snake and Ladder game state definitions and interface.
 *
 *  Created on: Sep 16, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_SNAKEANDLADDER_GAME_INT_H_
#define APP_SNAKEANDLADDER_GAME_INT_H_

#include "../../LIB/STD_TYPES.h"

typedef enum {
	SL_STATUS_WAITING,
	SL_STATUS_PLAYING,
	SL_STATUS_RESULT,
	SL_STATUS_ENDED
} SnakeLadder_Status;

typedef enum {
	SL_TURN_NONE,
	SL_TURN_P1,
	SL_TURN_P2
} SnakeLadder_Turn;

typedef enum {
	SL_WINNER_NONE,
	SL_WINNER_P1,
	SL_WINNER_P2
} SnakeLadder_Winner;

typedef struct {
	u8 Alive;
	SnakeLadder_Status Status;
	u8 Round;
	u8 Player1_Pos;
	u8 Player2_Pos;
	SnakeLadder_Turn Turn;
	u8 LastRoll;
	SnakeLadder_Winner Winner;
	char Event[20];
	char Room[12];
} SnakeLadder_ServerState;

void SnakeLadder_vStart(void);
void SnakeLadder_vUpdate(const SnakeLadder_ServerState *state);
void SnakeLadder_vStop(void);

#endif /* APP_SNAKEANDLADDER_GAME_INT_H_ */

