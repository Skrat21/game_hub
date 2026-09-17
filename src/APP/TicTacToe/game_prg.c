/*
 * game_prg.c
 *
 * Online Tic-Tac-Toe (XO) state manager — display mirror using BOARD_ANIM.
 *
 * Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#include "game_int.h"

#include "Animation/TicTacToe_Animation.h"

static XO_ServerState G_xLastState;
static u8 G_u8HasLastState = 0;

static u8 StringsEqual(const char *left, const char *right)
{
	while (*left != '\0' && *right != '\0' && *left == *right) {
		left++;
		right++;
	}
	return *left == '\0' && *right == '\0';
}

static u8 StateChanged(const XO_ServerState *state)
{
	return !G_u8HasLastState
			|| G_xLastState.Alive != state->Alive
			|| G_xLastState.Status != state->Status
			|| G_xLastState.Round != state->Round
			|| G_xLastState.Turn != state->Turn
			|| G_xLastState.Winner != state->Winner
			|| !StringsEqual(G_xLastState.Event, state->Event)
			|| !StringsEqual(G_xLastState.Room, state->Room);
}

static void CopyState(XO_ServerState *destination,
		const XO_ServerState *source)
{
	u8 index;

	destination->Alive = source->Alive;
	destination->Status = source->Status;
	destination->Round = source->Round;
	destination->Turn = source->Turn;
	destination->Winner = source->Winner;

	for (index = 0; index < XO_CELL_COUNT; index++) {
		destination->Board[index] = source->Board[index];
	}
	destination->Board[XO_CELL_COUNT - 1] = '\0';

	for (index = 0; index < sizeof(destination->Event) - 1
			&& source->Event[index] != '\0'; index++) {
		destination->Event[index] = source->Event[index];
	}
	destination->Event[index] = '\0';

	for (index = 0; index < sizeof(destination->Room) - 1
			&& source->Room[index] != '\0'; index++) {
		destination->Room[index] = source->Room[index];
	}
	destination->Room[index] = '\0';
}

void XO_vStart(void)
{
	G_u8HasLastState = 0;
	XO_Animation_vStart();
}

void XO_vUpdate(const XO_ServerState *state)
{
	if (state != NULL && StateChanged(state)) {
		CopyState(&G_xLastState, state);
		G_u8HasLastState = 1;
		XO_Animation_vShowState(state);
	}
}

void XO_vStop(void)
{
	G_u8HasLastState = 0;
	XO_Animation_vShowIdle();
}