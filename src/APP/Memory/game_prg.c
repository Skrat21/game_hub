/*
 * game_prg.c
 *
 * Online Memory matching game state manager.
 *
 * Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#include "game_int.h"

#include "Animation/Memory_Animation.h"

static Memory_ServerState G_xLastState;
static u8 G_u8HasLastState = 0;

static u8 StringsEqual(const char *left, const char *right)
{
	while (*left != '\0' && *right != '\0' && *left == *right) {
		left++;
		right++;
	}
	return *left == '\0' && *right == '\0';
}

static u8 StateChanged(const Memory_ServerState *state)
{
	return !G_u8HasLastState
			|| G_xLastState.Alive != state->Alive
			|| G_xLastState.Status != state->Status
			|| G_xLastState.Round != state->Round
			|| G_xLastState.Turn != state->Turn
			|| G_xLastState.Flip1 != state->Flip1
			|| G_xLastState.Flip2 != state->Flip2
			|| G_xLastState.MatchedPairs != state->MatchedPairs
			|| memcmp(G_xLastState.Board, state->Board, MEMORY_CELL_COUNT) != 0
			|| G_xLastState.Winner != state->Winner
			|| !StringsEqual(G_xLastState.Event, state->Event)
			|| !StringsEqual(G_xLastState.Room, state->Room);
}

static void CopyState(Memory_ServerState *destination,
		const Memory_ServerState *source)
{
	u8 index;

	destination->Alive = source->Alive;
	destination->Status = source->Status;
	destination->Round = source->Round;
	destination->Turn = source->Turn;
	destination->Flip1 = source->Flip1;
	destination->Flip2 = source->Flip2;
	destination->MatchedPairs = source->MatchedPairs;
	destination->Winner = source->Winner;

	for (index = 0; index < MEMORY_CELL_COUNT; index++) {
		destination->Board[index] = source->Board[index];
	}
	destination->Board[MEMORY_CELL_COUNT] = '\0';

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

void Memory_vStart(void)
{
	G_u8HasLastState = 0;
	Memory_Animation_vStart();
}

void Memory_vUpdate(const Memory_ServerState *state)
{
	if (state != NULL && StateChanged(state)) {
		CopyState(&G_xLastState, state);
		G_u8HasLastState = 1;
		Memory_Animation_vShowState(state);
	}
}

void Memory_vStop(void)
{
	G_u8HasLastState = 0;
	Memory_Animation_vShowIdle();
}