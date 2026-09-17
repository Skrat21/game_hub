/*
 * game_int.h
 *
 *  Created on: Sep 16, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_MATHQUIZ_GAME_INT_H_
#define APP_MATHQUIZ_GAME_INT_H_

#include "../../LIB/STD_TYPES.h"

typedef enum {
	MQ_STATUS_WAITING,
	MQ_STATUS_PLAYING,
	MQ_STATUS_RESULT,
	MQ_STATUS_ENDED
} MathQuiz_Status;

typedef enum {
	MQ_WINNER_NONE,
	MQ_WINNER_P1,
	MQ_WINNER_P2,
	MQ_WINNER_DRAW
} MathQuiz_Winner;

typedef struct {
	u8 Alive;
	MathQuiz_Status Status;
	u8 Round;
	char Question[16];
	u8 Player1_AnswerIsSet;
	char Player1_Answer[8];
	u8 Player2_AnswerIsSet;
	char Player2_Answer[8];
	MathQuiz_Winner Winner;
	char Event[20];
	char Room[12];
} MathQuiz_ServerState;

void MathQuiz_vStart(void);
void MathQuiz_vUpdate(const MathQuiz_ServerState *state);
void MathQuiz_vStop(void);

#endif /* APP_MATHQUIZ_GAME_INT_H_ */