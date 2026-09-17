/*
 * game_int.h
 *
 * Online Yatzy game — server state model.
 *
 * Protocol (website/Projecth/yatzy_backend.php, status/yatzy.txt):
 *   state=ALIVE;game=yatzy;room=<ID>;event=<EVENT>;status=waiting|playing|finished;
 *   turn=p1|p2|;rolls_left=<n>;dice=<d,d,d,d,d>;held_count=<n>;
 *   held_indices=<indices>;held_values=<values>;p1_score=<n>;p2_score=<n>;
 *   last_player=p1|p2|;last_action=<ACTION>;category_chosen=<CAT>;points_scored=<n>;time=..
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_YATZY_GAME_INT_H_
#define APP_YATZY_GAME_INT_H_

#include "../../LIB/STD_TYPES.h"

#define YATZY_DICE_COUNT 5
#define YATZY_MAX_SCORE 300  /* maximum points in an upper section */

typedef enum {
	YATZY_STATUS_WAITING,
	YATZY_STATUS_PLAYING,
	YATZY_STATUS_RESULT,
	YATZY_STATUS_ENDED
} Yatzy_Status;

typedef enum {
	YATZY_WINNER_NONE,
	YATZY_WINNER_P1,
	YATZY_WINNER_P2,
	YATZY_WINNER_DRAW
} Yatzy_Winner;

typedef struct {
	u8 Alive;
	Yatzy_Status Status;
	u8 Turn;               /* 1 or 2 */
	u8 RollsLeft;          /* 0-3 rolls per turn */
	u8 Dice[YATZY_DICE_COUNT];  /* 0 = not rolled yet, 1-6 = dice value */
	u8 HeldCount;          /* 0-5 held dice */
	u8 HeldIndices[YATZY_DICE_COUNT];  /* indices of held dice */
	u8 HeldValues[YATZY_DICE_COUNT];   /* values of held dice */
	u8 P1Score;
	u8 P2Score;
	char LastPlayer[4];    /* "p1" or "p2" */
	char LastAction[32];
	char CategoryChosen[32];
	u8 PointsScored;
	u8 Round;
	char Event[20];
	char Room[12];
} Yatzy_ServerState;

void Yatzy_vStart(void);
void Yatzy_vUpdate(const Yatzy_ServerState *state);
void Yatzy_vStop(void);

#endif /* APP_YATZY_GAME_INT_H_ */