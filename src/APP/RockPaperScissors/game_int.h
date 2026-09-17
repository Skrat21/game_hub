/*
 * game_int.h
 *
 *  Created on: Sep 13, 2026
 *      Author: AHMED SHERIF
 */


#ifndef APP_ROCKPAPERSCISSORS_GAME_INT_H_
#define APP_ROCKPAPERSCISSORS_GAME_INT_H_


#include "../../LIB/STD_TYPES.h"

typedef enum {
	RPS_CHOICE_NONE,
	ROCK,
	PAPER,
	SCISSORS,
}RPS_Choice;

typedef enum {
	RPS_STATUS_WAITING,
	RPS_STATUS_PLAYING,
	RPS_STATUS_RESULT,
	RPS_STATUS_ENDED
}RPS_Status;

typedef enum {
	RPS_WINNER_NONE,
	RPS_WINNER_P1,
	RPS_WINNER_P2,
	RPS_WINNER_DRAW
}RPS_Winner;

typedef struct {
	u8 Alive;
	RPS_Status Status;
	u8 Round;
	RPS_Choice Player1_Choice;
	RPS_Choice Player2_Choice;
	RPS_Winner Winner;
	char Event[20];
	char Room[12];
}RPS_ServerState;

void RPS_vStart(void);
void RPS_vUpdate(const RPS_ServerState *state);
void RPS_vStop(void);



#endif /* APP_ROCKPAPERSCISSORS_GAME_INT_H_ */
