#include "game_int.h"

#include "Animation/MathQuiz_Animation.h"

static MathQuiz_ServerState G_xLastState;
static u8 G_u8HasLastState = 0;

static u8 StringsEqual(const char *left, const char *right)
{
	while (*left != '\0' && *right != '\0' && *left == *right) {
		left++;
		right++;
	}
	return *left == '\0' && *right == '\0';
}

static u8 StateChanged(const MathQuiz_ServerState *state)
{
	return !G_u8HasLastState
			|| G_xLastState.Alive != state->Alive
			|| G_xLastState.Status != state->Status
			|| G_xLastState.Round != state->Round
			|| G_xLastState.Winner != state->Winner
			|| G_xLastState.Player1_AnswerIsSet != state->Player1_AnswerIsSet
			|| G_xLastState.Player2_AnswerIsSet != state->Player2_AnswerIsSet
			|| !StringsEqual(G_xLastState.Question, state->Question)
			|| !StringsEqual(G_xLastState.Player1_Answer, state->Player1_Answer)
			|| !StringsEqual(G_xLastState.Player2_Answer, state->Player2_Answer)
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

static void CopyState(MathQuiz_ServerState *destination,
		const MathQuiz_ServerState *source)
{
	u8 index;

	destination->Alive = source->Alive;
	destination->Status = source->Status;
	destination->Round = source->Round;
	destination->Winner = source->Winner;
	destination->Player1_AnswerIsSet = source->Player1_AnswerIsSet;
	destination->Player2_AnswerIsSet = source->Player2_AnswerIsSet;

	CopyString(destination->Question, source->Question,
			sizeof(destination->Question));
	CopyString(destination->Player1_Answer, source->Player1_Answer,
			sizeof(destination->Player1_Answer));
	CopyString(destination->Player2_Answer, source->Player2_Answer,
			sizeof(destination->Player2_Answer));
	CopyString(destination->Event, source->Event,
			sizeof(destination->Event));

	for (index = 0; index < sizeof(destination->Room) - 1
			&& source->Room[index] != '\0'; index++) {
		destination->Room[index] = source->Room[index];
	}
	destination->Room[index] = '\0';
}

void MathQuiz_vStart(void)
{
	G_u8HasLastState = 0;
	MathQuiz_Animation_vStart();
}

void MathQuiz_vUpdate(const MathQuiz_ServerState *state)
{
	if (state != NULL && StateChanged(state)) {
		CopyState(&G_xLastState, state);
		G_u8HasLastState = 1;
		MathQuiz_Animation_vShowState(state);
	}
}

void MathQuiz_vStop(void)
{
	G_u8HasLastState = 0;
	MathQuiz_Animation_vShowIdle();
}