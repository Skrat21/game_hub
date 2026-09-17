/*
 * game_prg.c
 *
 * Online Yatzy game state manager.
 *
 * Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#include "game_int.h"

#include <string.h>
#include "Animation/Yatzy_Animation.h"

static Yatzy_ServerState G_xYatzyLastState;
static u8 G_u8HasLastState = 0;

static u8 StringsEqual(const char *left, const char *right)
{
	while (*left != '\0' && *right != '\0' && *left == *right) {
		left++;
		right++;
	}
	return *left == '\0' && *right == '\0';
}

static u8 StateChanged(const Yatzy_ServerState *state)
{
	return !G_u8HasLastState
			|| G_xYatzyLastState.Alive != state->Alive
			|| G_xYatzyLastState.Status != state->Status
			|| G_xYatzyLastState.Round != state->Round
			|| G_xYatzyLastState.Turn != state->Turn
			|| G_xYatzyLastState.RollsLeft != state->RollsLeft
			|| memcmp(G_xYatzyLastState.Dice, state->Dice, YATZY_DICE_COUNT) != 0
			|| G_xYatzyLastState.HeldCount != state->HeldCount
			|| memcmp(G_xYatzyLastState.HeldIndices, state->HeldIndices, G_xYatzyLastState.HeldCount) != 0
			|| memcmp(G_xYatzyLastState.HeldValues, state->HeldValues, G_xYatzyLastState.HeldCount) != 0
			|| G_xYatzyLastState.P1Score != state->P1Score
			|| G_xYatzyLastState.P2Score != state->P2Score
			|| !StringsEqual(G_xYatzyLastState.Event, state->Event)
			|| !StringsEqual(G_xYatzyLastState.Room, state->Room)
			|| !StringsEqual(G_xYatzyLastState.LastAction, state->LastAction);
}

static void CopyState(Yatzy_ServerState *destination,
		const Yatzy_ServerState *source)
{
	u8 index;

	destination->Alive = source->Alive;
	destination->Status = source->Status;
	destination->Round = source->Round;
	destination->Turn = source->Turn;
	destination->RollsLeft = source->RollsLeft;
	destination->HeldCount = source->HeldCount;
	destination->P1Score = source->P1Score;
	destination->P2Score = source->P2Score;
	destination->P1Score = source->P1Score;
	destination->P2Score = source->P2Score;
	destination->PointsScored = source->PointsScored;

	for (index = 0; index < YATZY_DICE_COUNT; index++) {
		destination->Dice[index] = source->Dice[index];
	}

	for (index = 0; index < YATZY_DICE_COUNT; index++) {
		destination->HeldIndices[index] = 0;
		destination->HeldValues[index] = 0;
	}
	for (index = 0; index < source->HeldCount && index < YATZY_DICE_COUNT; index++) {
		destination->HeldIndices[index] = source->HeldIndices[index];
		destination->HeldValues[index] = source->HeldValues[index];
	}

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

	for (index = 0; index < sizeof(destination->LastAction) - 1
			&& source->LastAction[index] != '\0'; index++) {
		destination->LastAction[index] = source->LastAction[index];
	}
	destination->LastAction[index] = '\0';

	for (index = 0; index < sizeof(destination->CategoryChosen) - 1
			&& source->CategoryChosen[index] != '\0'; index++) {
		destination->CategoryChosen[index] = source->CategoryChosen[index];
	}
	destination->CategoryChosen[index] = '\0';
}

void Yatzy_vStart(void)
{
	G_u8HasLastState = 0;
	Yatzy_Animation_vStart();
}

void Yatzy_vUpdate(const Yatzy_ServerState *state)
{
	if (state != NULL && StateChanged(state)) {
		CopyState(&G_xYatzyLastState, state);
		G_u8HasLastState = 1;
		Yatzy_Animation_vShowState(state);
	}
}

void Yatzy_vStop(void)
{
	G_u8HasLastState = 0;
	Yatzy_Animation_vShowIdle();
}
