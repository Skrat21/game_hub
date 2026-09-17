/*
 * game_prg.c
 *
 * Online Connect 4 state manager.
 *
 * Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#include "game_int.h"

#include "Animation/Connect4_Animation.h"

static Connect4_ServerState G_xLastState;
static u8 G_u8HasLastState = 0;

static u8 StringsEqual(const char *left, const char *right)
{
	while (*left != '\0' && *right != '\0' && *left == *right) {
		left++;
		right++;
	}
	return *left == '\0' && *right == '\0';
}

static u8 StateChanged(const Connect4_ServerState *state)
{
	return !G_u8HasLastState
			|| G_xLastState.Alive != state->Alive
			|| G_xLastState.Status != state->Status
			|| G_xLastState.Round != state->Round
			|| G_xLastState.Turn != state->Turn
			|| G_xLastState.Winner != state->Winner
			|| memcmp(G_xLastState.Board, state->Board, CONNECT4_CELL_COUNT) != 0
			|| !StringsEqual(G_xLastState.Event, state->Event)
			|| !StringsEqual(G_xLastState.Room, state->Room);
}

static void CopyState(Connect4_ServerState *destination,
		const Connect4_ServerState *source)
{
	u8 index;

	destination->Alive = source->Alive;
	destination->Status = source->Status;
	destination->Round = source->Round;
	destination->Turn = source->Turn;
	destination->Winner = source->Winner;
	destination->LastMoveRow = source->LastMoveRow;
	destination->LastMoveCol = source->LastMoveCol;

	for (index = 0; index < CONNECT4_CELL_COUNT; index++) {
		destination->Board[index] = source->Board[index];
	}
	destination->Board[CONNECT4_CELL_COUNT] = '\0';

	for (index = 0; index < sizeof(destination->Room) - 1
			&& source->Room[index] != '\0'; index++) {
		destination->Room[index] = source->Room[index];
	}
	destination->Room[index] = '\0';

	for (index = 0; index < sizeof(destination->Event) - 1
			&& source->Event[index] != '\0'; index++) {
		destination->Event[index] = source->Event[index];
	}
	destination->Event[index] = '\0';

	for (index = 0; index < sizeof(destination->LastPlayer) - 1
			&& source->LastPlayer[index] != '\0'; index++) {
		destination->LastPlayer[index] = source->LastPlayer[index];
	}
	destination->LastPlayer[index] = '\0';

	/* Parse last_move_row/col if present in board context */
}

void Connect4_vStart(void)
{
	G_u8HasLastState = 0;
	Connect4_Animation_vStart();
}

void Connect4_vUpdate(const Connect4_ServerState *state)
{
	if (state != NULL && StateChanged(state)) {
		CopyState(&G_xLastState, state);
		G_u8HasLastState = 1;
		Connect4_Animation_vShowState(state);
	}
}

void Connect4_vStop(void)
{
	G_u8HasLastState = 0;
	Connect4_Animation_vShowIdle();
}