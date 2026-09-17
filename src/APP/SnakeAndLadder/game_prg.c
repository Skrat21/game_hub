/*
 * game_prg.c
 *
 * Snake and Ladder game state manager and delta dispatcher.
 *
 *  Created on: Sep 16, 2026
 *      Author: AHMED SHERIF
 */

#include "game_int.h"
#include "Animation/SnakeLadder_Animation.h"

static SnakeLadder_ServerState G_xLastState;
static u8 G_u8HasLastState = 0;

static u8 StringsEqual(const char *left, const char *right)
{
	while (*left != '\0' && *right != '\0' && *left == *right) {
		left++;
		right++;
	}
	return *left == '\0' && *right == '\0';
}

static u8 StateChanged(const SnakeLadder_ServerState *state)
{
	return !G_u8HasLastState
			|| G_xLastState.Alive != state->Alive
			|| G_xLastState.Status != state->Status
			|| G_xLastState.Round != state->Round
			|| G_xLastState.Player1_Pos != state->Player1_Pos
			|| G_xLastState.Player2_Pos != state->Player2_Pos
			|| G_xLastState.Turn != state->Turn
			|| G_xLastState.LastRoll != state->LastRoll
			|| G_xLastState.Winner != state->Winner
			|| !StringsEqual(G_xLastState.Event, state->Event)
			|| !StringsEqual(G_xLastState.Room, state->Room);
}

static void CopyString(char *destination, const char *source, u8 size)
{
	u8 index;

	for (index = 0; index < size - 1 && source[index] != '\0'; index++) {
		destination[index] = source[index];
	}
	destination[index] = '\0';
}

static void CopyState(SnakeLadder_ServerState *destination,
		const SnakeLadder_ServerState *source)
{
	destination->Alive = source->Alive;
	destination->Status = source->Status;
	destination->Round = source->Round;
	destination->Player1_Pos = source->Player1_Pos;
	destination->Player2_Pos = source->Player2_Pos;
	destination->Turn = source->Turn;
	destination->LastRoll = source->LastRoll;
	destination->Winner = source->Winner;

	CopyString(destination->Event, source->Event,
			sizeof(destination->Event));
	CopyString(destination->Room, source->Room,
			sizeof(destination->Room));
}

void SnakeLadder_vStart(void)
{
	G_u8HasLastState = 0;
	SnakeLadder_Animation_vStart();
}

void SnakeLadder_vUpdate(const SnakeLadder_ServerState *state)
{
	if (state == 0) {
		return;
	}

	if (!state->Alive && state->Status != SL_STATUS_ENDED) {
		SnakeLadder_Animation_vShowIdle();
		G_u8HasLastState = 0;
		return;
	}

	if (StateChanged(state)) {
		CopyState(&G_xLastState, state);
		G_u8HasLastState = 1;
		SnakeLadder_Animation_vShowState(state);
	}
}

void SnakeLadder_vStop(void)
{
	G_u8HasLastState = 0;
	SnakeLadder_Animation_vShowIdle();
}

