#include "game_int.h"

#include "Animation/RPS_Animation.h"

static RPS_ServerState G_xLastState;
static u8 G_u8HasLastState = 0;

static u8 StringsEqual(const char *left, const char *right)
{
	while (*left != '\0' && *right != '\0' && *left == *right) {
		left++;
		right++;
	}
	return *left == '\0' && *right == '\0';
}

static u8 StateChanged(const RPS_ServerState *state)
{
	return !G_u8HasLastState
			|| G_xLastState.Alive != state->Alive
			|| G_xLastState.Status != state->Status
			|| G_xLastState.Round != state->Round
			|| G_xLastState.Player1_Choice != state->Player1_Choice
			|| G_xLastState.Player2_Choice != state->Player2_Choice
			|| G_xLastState.Winner != state->Winner
			|| !StringsEqual(G_xLastState.Event, state->Event)
			|| !StringsEqual(G_xLastState.Room, state->Room);
}

static void CopyState(RPS_ServerState *destination,
		const RPS_ServerState *source)
{
	u8 index;

	destination->Alive = source->Alive;
	destination->Status = source->Status;
	destination->Round = source->Round;
	destination->Player1_Choice = source->Player1_Choice;
	destination->Player2_Choice = source->Player2_Choice;
	destination->Winner = source->Winner;

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

void RPS_vStart(void)
{
	G_u8HasLastState = 0;
	RPS_Animation_vStart();
}

void RPS_vUpdate(const RPS_ServerState *state)
{
	if (state != NULL && StateChanged(state)) {
		CopyState(&G_xLastState, state);
		G_u8HasLastState = 1;
		RPS_Animation_vShowState(state);
	}
}

void RPS_vStop(void)
{
	G_u8HasLastState = 0;
	RPS_Animation_vShowIdle();
}
