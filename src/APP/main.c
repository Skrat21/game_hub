#include "../LIB/BIT_MATH.h"
#include "../LIB/STD_TYPES.h"

#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/SYSTICK/SYSTICK_int.h"
#include "../MCAL/NVIC/NVIC_int.h"
#include "../MCAL/SPI/SPI_int.h"

#include "../HAL/TFT/TFT_int.h"
#include "../HAL/ESP8266/ESP8266_int.h"
#include "../HAL/ESP8266/ESP8266_cfg.h"
#include "../APP/OTA/OTA_int.h"
#include "RockPaperScissors/game_int.h"
#include "MathQuiz/game_int.h"
#include "SnakeAndLadder/game_int.h"
#include "../HAL/OTA_LED/OTA_LED_int.h"
#include "Menu/MENU_int.h"
#include "Menu/MENU_draw.h"
#include "Offline/OFFLINE_int.h"
#include "Connect4/game_int.h"
#include "TicTacToe/game_int.h"
#include "Memory/game_int.h"
#include "Yatzy/game_int.h"

#define HTTP_BODY_SIZE       512
#define CONNECTION_RETRIES   3
#define MAIN_STATUS_PATH     "/status/main.txt"
#define RPS_STATUS_PATH      "/status/rps.txt"
#define MATHQUIZ_STATUS_PATH "/status/mathquiz.txt"
#define SNAKELADDER_STATUS_PATH "/status/snakeladder.txt"
#define IDLE_DELAY_MS        1000
#define RPS_POLL_DELAY_MS    1000
#define MATHQUIZ_POLL_DELAY_MS 1000
#define SNAKELADDER_POLL_DELAY_MS 1000
#define RESULT_DISPLAY_MS    3000
#define XO_POLL_DELAY_MS     1000
#define CONNECT4_POLL_DELAY_MS 1000
#define MEMORY_POLL_DELAY_MS 1000
#define YATZY_POLL_DELAY_MS  1000
#define XO_STATUS_PATH       "/status/xo.txt"
#define CONNECT4_STATUS_PATH "/status/connect4.txt"
#define MEMORY_STATUS_PATH   "/status/memory.txt"
#define YATZY_STATUS_PATH    "/status/yatzy.txt"

static u8 StringEquals(const char *left, const char *right)
{
	while (*left != '\0' && *right != '\0' && *left == *right) {
		left++;
		right++;
	}
	return *left == '\0' && *right == '\0';
}

static u8 CopyField(const char *body, const char *field, char *value,
		u16 value_size)
{
	const char *cursor = body;
	u16 field_length = 0;

	while (field[field_length] != '\0') {
		field_length++;
	}

	while (*cursor != '\0') {
		const char *field_start = cursor;
		const char *value_start;
		u16 value_length = 0;
		u16 index;

		while (*cursor != '\0' && *cursor != '=' && *cursor != ';') {
			cursor++;
		}

		if (*cursor == '=' && (u16) (cursor - field_start) == field_length) {
			u8 matches = 1;
			for (index = 0; index < field_length; index++) {
				if (field_start[index] != field[index]) {
					matches = 0;
				}
			}

			value_start = ++cursor;
			while (*cursor != '\0' && *cursor != ';') {
				cursor++;
			}

			while (value_start + value_length < cursor) {
				value_length++;
			}

			if (matches && value_size > 0) {
				if (value_length >= value_size) {
					value_length = value_size - 1;
				}
				for (index = 0; index < value_length; index++) {
					value[index] = value_start[index];
				}
				value[value_length] = '\0';
				return 1;
			}
		} else {
			while (*cursor != '\0' && *cursor != ';') {
				cursor++;
			}
		}

		if (*cursor == ';') {
			cursor++;
		}
	}

	return 0;
}

static u8 ParseNumber(const char *text)
{
	u8 number = 0;

	while (*text >= '0' && *text <= '9') {
		number = (u8) (number * 10 + (*text - '0'));
		text++;
	}
	return number;
}

static RPS_Choice ParseChoice(const char *text)
{
	if (StringEquals(text, "rock")) {
		return ROCK;
	}
	if (StringEquals(text, "paper")) {
		return PAPER;
	}
	if (StringEquals(text, "scissors")) {
		return SCISSORS;
	}
	return RPS_CHOICE_NONE;
}

static RPS_Winner ParseWinner(const char *text)
{
	if (StringEquals(text, "p1")) {
		return RPS_WINNER_P1;
	}
	if (StringEquals(text, "p2")) {
		return RPS_WINNER_P2;
	}
	if (StringEquals(text, "draw")) {
		return RPS_WINNER_DRAW;
	}
	return RPS_WINNER_NONE;
}

static u8 IsRpsGame(const char *body)
{
	char state[16];
	char game[16];

	return CopyField(body, "state", state, sizeof(state))
			&& CopyField(body, "game", game, sizeof(game))
			&& StringEquals(state, "ALIVE") && StringEquals(game, "rps");
}

static u8 ParseRpsState(const char *body, RPS_ServerState *state)
{
	char lifecycle[12];
	char status[16];
	char choice[16];
	char winner[16];
	char round[8];

	state->Alive = 0;
	state->Status = RPS_STATUS_ENDED;
	state->Player1_Choice = RPS_CHOICE_NONE;
	state->Player2_Choice = RPS_CHOICE_NONE;
	state->Winner = RPS_WINNER_NONE;
	state->Round = 0;
	state->Event[0] = '\0';
	state->Room[0] = '\0';

	if (!CopyField(body, "state", lifecycle, sizeof(lifecycle))
			|| !CopyField(body, "status", status, sizeof(status))) {
		return 0;
	}

	state->Alive = StringEquals(lifecycle, "ALIVE");
	if (CopyField(body, "room", state->Room, sizeof(state->Room))) {
		/* Room is optional for the display, but useful for diagnostics. */
	}
	CopyField(body, "event", state->Event, sizeof(state->Event));

	if (!state->Alive || StringEquals(status, "dead")
			|| StringEquals(status, "ended")) {
		state->Status = RPS_STATUS_ENDED;
	} else if (StringEquals(status, "waiting")) {
		state->Status = RPS_STATUS_WAITING;
	} else if (StringEquals(status, "result")
			|| StringEquals(status, "finished")) {
		state->Status = RPS_STATUS_RESULT;
	} else {
		state->Status = RPS_STATUS_PLAYING;
	}

	if (CopyField(body, "p1_choice", choice, sizeof(choice))) {
		state->Player1_Choice = ParseChoice(choice);
	}
	if (CopyField(body, "p2_choice", choice, sizeof(choice))) {
		state->Player2_Choice = ParseChoice(choice);
	}
	if (CopyField(body, "winner", winner, sizeof(winner))) {
		state->Winner = ParseWinner(winner);
	}
	if (CopyField(body, "round", round, sizeof(round))) {
		state->Round = ParseNumber(round);
	}
	if (StringEquals(state->Event, "round_result")) {
		state->Status = RPS_STATUS_RESULT;
	}

	return state->Alive || state->Status == RPS_STATUS_ENDED;
}

static void CopyString(char *destination, const char *source, u16 size)
{
	u16 index;

	for (index = 0; index < size - 1 && source[index] != '\0'; index++) {
		destination[index] = source[index];
	}
	destination[index] = '\0';
}

static MathQuiz_Winner ParseMathQuizWinner(const char *text)
{
	if (StringEquals(text, "p1")) {
		return MQ_WINNER_P1;
	}
	if (StringEquals(text, "p2")) {
		return MQ_WINNER_P2;
	}
	if (StringEquals(text, "draw")) {
		return MQ_WINNER_DRAW;
	}
	return MQ_WINNER_NONE;
}

static u8 IsMathQuizGame(const char *body)
{
	char state[16];
	char game[16];

	return CopyField(body, "state", state, sizeof(state))
			&& CopyField(body, "game", game, sizeof(game))
			&& StringEquals(state, "ALIVE") && StringEquals(game, "mathquiz");
}

static u8 ParseMathQuizState(const char *body, MathQuiz_ServerState *state)
{
	char lifecycle[12];
	char status[16];
	char answer[8];
	char winner[16];
	char round[8];
	char question[16];

	state->Alive = 0;
	state->Status = MQ_STATUS_ENDED;
	state->Round = 0;
	state->Question[0] = '\0';
	state->Player1_AnswerIsSet = 0;
	state->Player1_Answer[0] = '\0';
	state->Player2_AnswerIsSet = 0;
	state->Player2_Answer[0] = '\0';
	state->Winner = MQ_WINNER_NONE;
	state->Event[0] = '\0';
	state->Room[0] = '\0';

	if (!CopyField(body, "state", lifecycle, sizeof(lifecycle))
			|| !CopyField(body, "status", status, sizeof(status))) {
		return 0;
	}

	state->Alive = StringEquals(lifecycle, "ALIVE");
	CopyField(body, "room", state->Room, sizeof(state->Room));
	CopyField(body, "event", state->Event, sizeof(state->Event));
	if (CopyField(body, "question", question, sizeof(question))) {
		CopyString(state->Question, question, sizeof(state->Question));
	}

	if (!state->Alive || StringEquals(status, "ended")) {
		state->Status = MQ_STATUS_ENDED;
	} else if (StringEquals(status, "waiting")) {
		state->Status = MQ_STATUS_WAITING;
	} else if (StringEquals(status, "result")) {
		state->Status = MQ_STATUS_RESULT;
	} else {
		state->Status = MQ_STATUS_PLAYING;
	}
	if (StringEquals(state->Event, "match_ended")) {
		state->Status = MQ_STATUS_ENDED;
	}

	if (CopyField(body, "round", round, sizeof(round))) {
		state->Round = ParseNumber(round);
	}
	if (CopyField(body, "winner", winner, sizeof(winner))) {
		state->Winner = ParseMathQuizWinner(winner);
	}

	if (CopyField(body, "p1_answer", answer, sizeof(answer))
			&& answer[0] != '\0') {
		state->Player1_AnswerIsSet = 1;
		CopyString(state->Player1_Answer, answer,
				sizeof(state->Player1_Answer));
	}
	if (CopyField(body, "p2_answer", answer, sizeof(answer))
			&& answer[0] != '\0') {
		state->Player2_AnswerIsSet = 1;
		CopyString(state->Player2_Answer, answer,
				sizeof(state->Player2_Answer));
	}

	return state->Alive || state->Status == MQ_STATUS_ENDED;
}

static SnakeLadder_Winner ParseSnakeLadderWinner(const char *text)
{
	if (StringEquals(text, "p1")) {
		return SL_WINNER_P1;
	}
	if (StringEquals(text, "p2")) {
		return SL_WINNER_P2;
	}
	return SL_WINNER_NONE;
}

static SnakeLadder_Turn ParseSnakeLadderTurn(const char *text)
{
	if (StringEquals(text, "p1") || StringEquals(text, "P1")) {
		return SL_TURN_P1;
	}
	if (StringEquals(text, "p2") || StringEquals(text, "P2")) {
		return SL_TURN_P2;
	}
	return SL_TURN_NONE;
}

static u8 IsSnakeLadderGame(const char *body)
{
	char state[16];
	char game[16];

	return CopyField(body, "state", state, sizeof(state))
			&& CopyField(body, "game", game, sizeof(game))
			&& StringEquals(state, "ALIVE") && StringEquals(game, "snakeladder");
}

static u8 ParseSnakeLadderState(const char *body, SnakeLadder_ServerState *state)
{
	char lifecycle[12];
	char status[16];
	char round[8];
	char p1_pos[8];
	char p2_pos[8];
	char turn[8];
	char last_roll[8];
	char winner[16];

	state->Alive = 0;
	state->Status = SL_STATUS_ENDED;
	state->Round = 0;
	state->Player1_Pos = 0;
	state->Player2_Pos = 0;
	state->Turn = SL_TURN_NONE;
	state->LastRoll = 0;
	state->Winner = SL_WINNER_NONE;
	state->Event[0] = '\0';
	state->Room[0] = '\0';

	if (!CopyField(body, "state", lifecycle, sizeof(lifecycle))
			|| !CopyField(body, "status", status, sizeof(status))) {
		return 0;
	}

	state->Alive = StringEquals(lifecycle, "ALIVE");
	CopyField(body, "room", state->Room, sizeof(state->Room));
	CopyField(body, "event", state->Event, sizeof(state->Event));

	if (!state->Alive || StringEquals(status, "ended") || StringEquals(status, "dead")) {
		state->Status = SL_STATUS_ENDED;
	} else if (StringEquals(status, "waiting")) {
		state->Status = SL_STATUS_WAITING;
	} else if (StringEquals(status, "result")) {
		state->Status = SL_STATUS_RESULT;
	} else {
		state->Status = SL_STATUS_PLAYING;
	}

	if (StringEquals(state->Event, "match_ended")) {
		state->Status = SL_STATUS_ENDED;
	}

	if (CopyField(body, "round", round, sizeof(round))) {
		state->Round = ParseNumber(round);
	}
	if (CopyField(body, "p1_pos", p1_pos, sizeof(p1_pos))) {
		state->Player1_Pos = ParseNumber(p1_pos);
	}
	if (CopyField(body, "p2_pos", p2_pos, sizeof(p2_pos))) {
		state->Player2_Pos = ParseNumber(p2_pos);
	}
	if (CopyField(body, "turn", turn, sizeof(turn))) {
		state->Turn = ParseSnakeLadderTurn(turn);
	}
	if (CopyField(body, "last_roll", last_roll, sizeof(last_roll))) {
		state->LastRoll = ParseNumber(last_roll);
	}
	if (CopyField(body, "winner", winner, sizeof(winner))) {
		state->Winner = ParseSnakeLadderWinner(winner);
	}

	return state->Alive || state->Status == SL_STATUS_ENDED;
}

static XO_Mark ParseXOMark(const char *text)
{
	if (StringEquals(text, "X")) return XO_MARK_X;
	if (StringEquals(text, "O")) return XO_MARK_O;
	return XO_MARK_NONE;
}

static XO_Winner ParseXOWinner(const char *text)
{
	if (StringEquals(text, "X")) return XO_WINNER_X;
	if (StringEquals(text, "O")) return XO_WINNER_O;
	if (StringEquals(text, "draw")) return XO_WINNER_DRAW;
	return XO_WINNER_NONE;
}

static u8 IsXOGame(const char *body)
{
	char state[16]; char game[16];
	return CopyField(body, "state", state, sizeof(state))
		&& CopyField(body, "game", game, sizeof(game))
		&& StringEquals(state, "ALIVE") && StringEquals(game, "xo");
}

static u8 ParseXOState(const char *body, XO_ServerState *state)
{
	char lifecycle[12], status[16], turn[8], winner[16], round[8];
	state->Alive = 0; state->Status = XO_STATUS_ENDED; state->Round = 0;
	state->Turn = XO_MARK_NONE; state->Winner = XO_WINNER_NONE;
	state->Board[0] = '\0'; state->Event[0] = '\0'; state->Room[0] = '\0';
	if (!CopyField(body, "state", lifecycle, sizeof(lifecycle))
		|| !CopyField(body, "status", status, sizeof(status))) return 0;
	state->Alive = StringEquals(lifecycle, "ALIVE");
	CopyField(body, "room", state->Room, sizeof(state->Room));
	CopyField(body, "event", state->Event, sizeof(state->Event));
	if (!state->Alive || StringEquals(status, "ended") || StringEquals(status, "dead"))
		state->Status = XO_STATUS_ENDED;
	else if (StringEquals(status, "waiting")) state->Status = XO_STATUS_WAITING;
	else if (StringEquals(status, "result")) state->Status = XO_STATUS_RESULT;
	else state->Status = XO_STATUS_PLAYING;
	if (CopyField(body, "board", state->Board, XO_CELL_COUNT + 1))
		state->Board[XO_CELL_COUNT] = '\0';
	if (CopyField(body, "turn", turn, sizeof(turn))) state->Turn = ParseXOMark(turn);
	if (CopyField(body, "winner", winner, sizeof(winner))) state->Winner = ParseXOWinner(winner);
	if (CopyField(body, "round", round, sizeof(round))) state->Round = ParseNumber(round);
	if (StringEquals(state->Event, "round_result")) state->Status = XO_STATUS_RESULT;
	return state->Alive || state->Status == XO_STATUS_ENDED;
}

static Connect4_Mark ParseConnect4Mark(const char *text)
{
	if (StringEquals(text, "p1")) return CONNECT4_MARK_RED;
	if (StringEquals(text, "p2")) return CONNECT4_MARK_YELLOW;
	return CONNECT4_MARK_NONE;
}

static Connect4_Winner ParseConnect4Winner(const char *text)
{
	if (StringEquals(text, "p1")) return CONNECT4_WINNER_P1;
	if (StringEquals(text, "p2")) return CONNECT4_WINNER_P2;
	if (StringEquals(text, "draw")) return CONNECT4_WINNER_DRAW;
	return CONNECT4_WINNER_NONE;
}

static u8 IsConnect4Game(const char *body)
{
	char state[16]; char game[16];
	return CopyField(body, "state", state, sizeof(state))
		&& CopyField(body, "game", game, sizeof(game))
		&& StringEquals(state, "ALIVE") && StringEquals(game, "connect4");
}

static u8 ParseConnect4State(const char *body, Connect4_ServerState *state)
{
	char lifecycle[12], status[16], turn[8], winner[16], round[8];
	state->Alive = 0; state->Status = CONNECT4_STATUS_ENDED; state->Round = 0;
	state->Turn = CONNECT4_MARK_NONE; state->Winner = CONNECT4_WINNER_NONE;
	state->LastMoveRow = 0; state->LastMoveCol = 0;
	state->Board[0] = '\0'; state->Event[0] = '\0'; state->Room[0] = '\0';
	state->LastPlayer[0] = '\0';
	if (!CopyField(body, "state", lifecycle, sizeof(lifecycle))
		|| !CopyField(body, "status", status, sizeof(status))) return 0;
	state->Alive = StringEquals(lifecycle, "ALIVE");
	CopyField(body, "room", state->Room, sizeof(state->Room));
	CopyField(body, "event", state->Event, sizeof(state->Event));
	CopyField(body, "last_player", state->LastPlayer, sizeof(state->LastPlayer));
	if (!state->Alive || StringEquals(status, "ended") || StringEquals(status, "dead"))
		state->Status = CONNECT4_STATUS_ENDED;
	else if (StringEquals(status, "waiting")) state->Status = CONNECT4_STATUS_WAITING;
	else if (StringEquals(status, "result")) state->Status = CONNECT4_STATUS_RESULT;
	else state->Status = CONNECT4_STATUS_PLAYING;
	if (CopyField(body, "board", state->Board, CONNECT4_CELL_COUNT + 1))
		state->Board[CONNECT4_CELL_COUNT] = '\0';
	if (CopyField(body, "turn", turn, sizeof(turn))) state->Turn = ParseConnect4Mark(turn);
	if (CopyField(body, "winner", winner, sizeof(winner))) state->Winner = ParseConnect4Winner(winner);
	if (CopyField(body, "round", round, sizeof(round))) state->Round = ParseNumber(round);
	if (CopyField(body, "last_move_row", round, sizeof(round))) state->LastMoveRow = (u8)ParseNumber(round);
	if (CopyField(body, "last_move_column", round, sizeof(round))) state->LastMoveCol = (u8)ParseNumber(round);
	return state->Alive || state->Status == CONNECT4_STATUS_ENDED;
}

static Memory_Winner ParseMemoryWinner(const char *text)
{
	if (StringEquals(text, "p1")) return MEMORY_WINNER_P1;
	if (StringEquals(text, "p2")) return MEMORY_WINNER_P2;
	if (StringEquals(text, "draw")) return MEMORY_WINNER_DRAW;
	return MEMORY_WINNER_NONE;
}

static u8 IsMemoryGame(const char *body)
{
	char state[16]; char game[16];
	return CopyField(body, "state", state, sizeof(state))
		&& CopyField(body, "game", game, sizeof(game))
		&& StringEquals(state, "ALIVE") && StringEquals(game, "memory");
}

static u8 ParseMemoryState(const char *body, Memory_ServerState *state)
{
	char lifecycle[12], status[16], turn[8], flip1[8], flip2[8];
	char matched_pairs[8], winner[16], round[8];
	state->Alive = 0; state->Status = MEMORY_STATUS_ENDED; state->Round = 0;
	state->Turn = 0; state->Flip1 = 0xFF; state->Flip2 = 0xFF;
	state->MatchedPairs = 0; state->Winner = MEMORY_WINNER_NONE;
	state->Board[0] = '\0'; state->Event[0] = '\0'; state->Room[0] = '\0';
	if (!CopyField(body, "state", lifecycle, sizeof(lifecycle))
		|| !CopyField(body, "status", status, sizeof(status))) return 0;
	state->Alive = StringEquals(lifecycle, "ALIVE");
	CopyField(body, "room", state->Room, sizeof(state->Room));
	CopyField(body, "event", state->Event, sizeof(state->Event));
	if (!state->Alive || StringEquals(status, "ended") || StringEquals(status, "dead"))
		state->Status = MEMORY_STATUS_ENDED;
	else if (StringEquals(status, "waiting")) state->Status = MEMORY_STATUS_WAITING;
	else if (StringEquals(status, "result")) state->Status = MEMORY_STATUS_RESULT;
	else state->Status = MEMORY_STATUS_PLAYING;
	if (CopyField(body, "board", state->Board, MEMORY_CELL_COUNT + 1))
		state->Board[MEMORY_CELL_COUNT] = '\0';
	if (CopyField(body, "turn", turn, sizeof(turn))) {
		if (StringEquals(turn, "p1")) state->Turn = 1;
		else if (StringEquals(turn, "p2")) state->Turn = 2;
	}
	if (CopyField(body, "flip1", flip1, sizeof(flip1))) state->Flip1 = (u8)ParseNumber(flip1);
	if (CopyField(body, "flip2", flip2, sizeof(flip2))) state->Flip2 = (u8)ParseNumber(flip2);
	if (CopyField(body, "matched_pairs", matched_pairs, sizeof(matched_pairs)))
		state->MatchedPairs = (u8)ParseNumber(matched_pairs);
	if (CopyField(body, "winner", winner, sizeof(winner))) state->Winner = ParseMemoryWinner(winner);
	if (CopyField(body, "round", round, sizeof(round))) state->Round = (u8)ParseNumber(round);
	return state->Alive || state->Status == MEMORY_STATUS_ENDED;
}

static u8 IsYatzyGame(const char *body)
{
	char state[16]; char game[16];
	return CopyField(body, "state", state, sizeof(state))
		&& CopyField(body, "game", game, sizeof(game))
		&& StringEquals(state, "ALIVE") && StringEquals(game, "yatzy");
}

static u8 ParseYatzyState(const char *body, Yatzy_ServerState *state)
{
	char lifecycle[12], status[16], turn[8], rolls_left[8], dice[32];
	char held_count[8], held_indices[32], held_values[32];
	char p1_score[8], p2_score[8], last_player[8], last_action[32];
	char category_chosen[32], points_scored[8];
	state->Alive = 0; state->Status = YATZY_STATUS_ENDED; state->Round = 0;
	state->Turn = 0; state->RollsLeft = 0; state->HeldCount = 0;
	state->P1Score = 0; state->P2Score = 0;
	state->PointsScored = 0;
	state->LastPlayer[0] = '\0'; state->LastAction[0] = '\0';
	state->CategoryChosen[0] = '\0';
	state->Dice[0]=state->Dice[1]=state->Dice[2]=state->Dice[3]=state->Dice[4]=0;
	state->HeldIndices[0]=state->HeldIndices[1]=state->HeldIndices[2]=state->HeldIndices[3]=state->HeldIndices[4]=0;
	state->HeldValues[0]=state->HeldValues[1]=state->HeldValues[2]=state->HeldValues[3]=state->HeldValues[4]=0;
	state->Event[0] = '\0'; state->Room[0] = '\0';
	if (!CopyField(body, "state", lifecycle, sizeof(lifecycle))
		|| !CopyField(body, "status", status, sizeof(status))) return 0;
	state->Alive = StringEquals(lifecycle, "ALIVE");
	CopyField(body, "room", state->Room, sizeof(state->Room));
	CopyField(body, "event", state->Event, sizeof(state->Event));
	if (!state->Alive || StringEquals(status, "ended") || StringEquals(status, "dead")
		|| StringEquals(status, "finished"))
		state->Status = YATZY_STATUS_ENDED;
	else if (StringEquals(status, "waiting")) state->Status = YATZY_STATUS_WAITING;
	else if (StringEquals(status, "result")) state->Status = YATZY_STATUS_RESULT;
	else state->Status = YATZY_STATUS_PLAYING;
	if (CopyField(body, "turn", turn, sizeof(turn))) {
		if (StringEquals(turn, "p1")) state->Turn = 1;
		else if (StringEquals(turn, "p2")) state->Turn = 2;
	}
	if (CopyField(body, "rolls_left", rolls_left, sizeof(rolls_left)))
		state->RollsLeft = (u8)ParseNumber(rolls_left);
	if (CopyField(body, "dice", dice, sizeof(dice))) {
		u8 d = 0; const char *p = dice;
		while (*p && d < YATZY_DICE_COUNT) {
			while (*p == ' ') p++;
			if (*p >= '1' && *p <= '6') { state->Dice[d] = (u8)(*p - '0'); d++; }
			while (*p && *p != ',') p++;
			if (*p == ',') p++;
		}
	}
	if (CopyField(body, "held_count", held_count, sizeof(held_count)))
		state->HeldCount = (u8)ParseNumber(held_count);
	if (CopyField(body, "held_indices", held_indices, sizeof(held_indices))) {
		u8 i = 0; const char *p = held_indices;
		while (*p && i < YATZY_DICE_COUNT) {
			while (*p == ' ') p++;
			if (*p >= '0' && *p <= '9') { state->HeldIndices[i] = (u8)ParseNumber(p); i++; }
			while (*p && *p != ',') p++;
			if (*p == ',') p++;
		}
	}
	if (CopyField(body, "held_values", held_values, sizeof(held_values))) {
		u8 v = 0; const char *p = held_values;
		while (*p && v < YATZY_DICE_COUNT) {
			while (*p == ' ') p++;
			if (*p >= '1' && *p <= '6') { state->HeldValues[v] = (u8)(*p - '0'); v++; }
			while (*p && *p != ',') p++;
			if (*p == ',') p++;
		}
	}
	if (CopyField(body, "p1_score", p1_score, sizeof(p1_score)))
		state->P1Score = (u8)ParseNumber(p1_score);
	if (CopyField(body, "p2_score", p2_score, sizeof(p2_score)))
		state->P2Score = (u8)ParseNumber(p2_score);
	if (CopyField(body, "last_player", last_player, sizeof(last_player)))
		CopyString(state->LastPlayer, last_player, sizeof(state->LastPlayer));
	if (CopyField(body, "last_action", last_action, sizeof(last_action)))
		CopyString(state->LastAction, last_action, sizeof(state->LastAction));
	if (CopyField(body, "category_chosen", category_chosen, sizeof(category_chosen)))
		CopyString(state->CategoryChosen, category_chosen, sizeof(state->CategoryChosen));
	if (CopyField(body, "points_scored", points_scored, sizeof(points_scored)))
		state->PointsScored = (u8)ParseNumber(points_scored);
	if (CopyField(body, "round", rolls_left, sizeof(rolls_left)))
		state->Round = (u8)ParseNumber(rolls_left);
	return state->Alive || state->Status == YATZY_STATUS_ENDED;
}

static void InitHardware(void)
{
	MSYSTIC_Config_t systick_config = {
		.InterruptEnable = INT_DISABLE,
		.CLK_SRC = CLK_SRC_AHB_8
	};
	GPIOx_PinConfig_t sck = {
		.Port = GPIO_PORTA, .Pin = GPIO_PIN5,
		.Mode = GPIO_MODE_ALF, .AltFunc = GPIO_AF5
	};
	GPIOx_PinConfig_t mosi = {
		.Port = GPIO_PORTA, .Pin = GPIO_PIN7,
		.Mode = GPIO_MODE_ALF, .AltFunc = GPIO_AF5
	};
	GPIOx_PinConfig_t tx = {
		.Port = GPIO_PORTA, .Pin = GPIO_PIN9,
		.Mode = GPIO_MODE_ALF, .AltFunc = GPIO_AF7
	};
	GPIOx_PinConfig_t rx = {
		.Port = GPIO_PORTA, .Pin = GPIO_PIN10,
		.Mode = GPIO_MODE_ALF, .AltFunc = GPIO_AF7
	};

	MRCC_vInit();
	MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);
	MRCC_vEnableCLK(RCC_APB2, 4);
	MRCC_vEnableCLK(RCC_APB2, 12);
	MSYSTICK_vInit(&systick_config);
	MSPI_vInit();
	MGPIO_vPinInit(&sck);
	MGPIO_vPinInit(&mosi);
	MGPIO_vPinInit(&tx);
	MGPIO_vPinInit(&rx);
	MNVIC_vEnable_Peripheral_INT(37);
	HTFT_vInit();
	HTFT_vFillBackgroundColor(TFT_BLACK);
	OTA_LED_vInit();
}

static void RunOnlineMode(void)
{
	char body[HTTP_BODY_SIZE];
	RPS_ServerState rps_state;
	MathQuiz_ServerState mathquiz_state;
	SnakeLadder_ServerState snakeladder_state;
	XO_ServerState xo_state;
	Connect4_ServerState connect4_state;
	Memory_ServerState memory_state;
	Yatzy_ServerState yatzy_state;

	HESP8266_vInit();

	/* Check for firmware update at boot */
	OTA_vCheckForUpdate();

	while (1) {
		if (HESP8266_u8EnsureConnection(HESP8266_WIFI_SSID,
				HESP8266_WIFI_PASSWORD, HESP8266_SERVER_IP,
				HESP8266_SERVER_PORT, CONNECTION_RETRIES)
				!= HESP8266_STATUS_OK) {
			HESP8266_vAbort();
			MSYSTICK_vSetDelay_ms(IDLE_DELAY_MS);
			continue;
		}

		G_xHESP8266DebugStage = HESP8266_DEBUG_MAIN_STATUS_SENT;
		if (HESP8266_u8HttpGet(MAIN_STATUS_PATH, body, sizeof(body))
				!= HESP8266_STATUS_OK) {
			HESP8266_vAbort();
			continue;
		}
		G_xHESP8266DebugStage = HESP8266_DEBUG_MAIN_STATUS_RECEIVED;
		HESP8266_vCloseConnection();

		if (IsMathQuizGame(body)) {
			MathQuiz_vStart();
			while (1) {
				if (HESP8266_u8EnsureConnection(HESP8266_WIFI_SSID,
						HESP8266_WIFI_PASSWORD, HESP8266_SERVER_IP,
						HESP8266_SERVER_PORT, CONNECTION_RETRIES)
						!= HESP8266_STATUS_OK) {
					HESP8266_vAbort();
					MSYSTICK_vSetDelay_ms(MATHQUIZ_POLL_DELAY_MS);
					continue;
				}

				if (HESP8266_u8HttpGet(MATHQUIZ_STATUS_PATH, body,
						sizeof(body)) != HESP8266_STATUS_OK) {
					HESP8266_vAbort();
					MSYSTICK_vSetDelay_ms(MATHQUIZ_POLL_DELAY_MS);
					continue;
				}
				HESP8266_vCloseConnection();

				if (!ParseMathQuizState(body, &mathquiz_state)) {
					MSYSTICK_vSetDelay_ms(MATHQUIZ_POLL_DELAY_MS);
					continue;
				}
				MathQuiz_vUpdate(&mathquiz_state);
				if (!mathquiz_state.Alive
						|| mathquiz_state.Status == MQ_STATUS_ENDED) {
					if (mathquiz_state.Status == MQ_STATUS_ENDED) {
						MSYSTICK_vSetDelay_ms(RESULT_DISPLAY_MS);
					}
					break;
				}
				MSYSTICK_vSetDelay_ms(MATHQUIZ_POLL_DELAY_MS);
			}

			HESP8266_vAbort();
			MathQuiz_vStop();

			/* Check for firmware update between games */
			OTA_vCheckForUpdate();
			continue;
		}

		if (IsSnakeLadderGame(body)) {
			SnakeLadder_vStart();
			while (1) {
				if (HESP8266_u8EnsureConnection(HESP8266_WIFI_SSID,
						HESP8266_WIFI_PASSWORD, HESP8266_SERVER_IP,
						HESP8266_SERVER_PORT, CONNECTION_RETRIES)
						!= HESP8266_STATUS_OK) {
					HESP8266_vAbort();
					MSYSTICK_vSetDelay_ms(SNAKELADDER_POLL_DELAY_MS);
					continue;
				}

				if (HESP8266_u8HttpGet(SNAKELADDER_STATUS_PATH, body,
						sizeof(body)) != HESP8266_STATUS_OK) {
					HESP8266_vAbort();
					MSYSTICK_vSetDelay_ms(SNAKELADDER_POLL_DELAY_MS);
					continue;
				}
				HESP8266_vCloseConnection();

				if (!ParseSnakeLadderState(body, &snakeladder_state)) {
					MSYSTICK_vSetDelay_ms(SNAKELADDER_POLL_DELAY_MS);
					continue;
				}
				SnakeLadder_vUpdate(&snakeladder_state);
				if (!snakeladder_state.Alive
						|| snakeladder_state.Status == SL_STATUS_ENDED) {
					if (snakeladder_state.Status == SL_STATUS_ENDED) {
						MSYSTICK_vSetDelay_ms(RESULT_DISPLAY_MS);
					}
					break;
				}
				MSYSTICK_vSetDelay_ms(SNAKELADDER_POLL_DELAY_MS);
			}

			HESP8266_vAbort();
			SnakeLadder_vStop();

			/* Check for firmware update between games */
			OTA_vCheckForUpdate();
			continue;
		}

		if (IsXOGame(body)) {
			XO_vStart();
			while (1) {
				if (HESP8266_u8EnsureConnection(HESP8266_WIFI_SSID,
						HESP8266_WIFI_PASSWORD, HESP8266_SERVER_IP,
						HESP8266_SERVER_PORT, CONNECTION_RETRIES)
						!= HESP8266_STATUS_OK) {
					HESP8266_vAbort();
					MSYSTICK_vSetDelay_ms(XO_POLL_DELAY_MS);
					continue;
				}

				if (HESP8266_u8HttpGet(XO_STATUS_PATH, body,
						sizeof(body)) != HESP8266_STATUS_OK) {
					HESP8266_vAbort();
					MSYSTICK_vSetDelay_ms(XO_POLL_DELAY_MS);
					continue;
				}
				HESP8266_vCloseConnection();

				if (!ParseXOState(body, &xo_state)) {
					MSYSTICK_vSetDelay_ms(XO_POLL_DELAY_MS);
					continue;
				}
				XO_vUpdate(&xo_state);
				if (!xo_state.Alive
						|| xo_state.Status == XO_STATUS_ENDED) {
					if (xo_state.Status == XO_STATUS_ENDED) {
						MSYSTICK_vSetDelay_ms(RESULT_DISPLAY_MS);
					}
					break;
				}
				MSYSTICK_vSetDelay_ms(XO_POLL_DELAY_MS);
			}

			HESP8266_vAbort();
			XO_vStop();

			/* Check for firmware update between games */
			OTA_vCheckForUpdate();
			continue;
		}

		if (IsConnect4Game(body)) {
			Connect4_vStart();
			while (1) {
				if (HESP8266_u8EnsureConnection(HESP8266_WIFI_SSID,
						HESP8266_WIFI_PASSWORD, HESP8266_SERVER_IP,
						HESP8266_SERVER_PORT, CONNECTION_RETRIES)
						!= HESP8266_STATUS_OK) {
					HESP8266_vAbort();
					MSYSTICK_vSetDelay_ms(CONNECT4_POLL_DELAY_MS);
					continue;
				}

				if (HESP8266_u8HttpGet(CONNECT4_STATUS_PATH, body,
						sizeof(body)) != HESP8266_STATUS_OK) {
					HESP8266_vAbort();
					MSYSTICK_vSetDelay_ms(CONNECT4_POLL_DELAY_MS);
					continue;
				}
				HESP8266_vCloseConnection();

				if (!ParseConnect4State(body, &connect4_state)) {
					MSYSTICK_vSetDelay_ms(CONNECT4_POLL_DELAY_MS);
					continue;
				}
				Connect4_vUpdate(&connect4_state);
				if (!connect4_state.Alive
						|| connect4_state.Status == CONNECT4_STATUS_ENDED) {
					if (connect4_state.Status == CONNECT4_STATUS_ENDED) {
						MSYSTICK_vSetDelay_ms(RESULT_DISPLAY_MS);
					}
					break;
				}
				MSYSTICK_vSetDelay_ms(CONNECT4_POLL_DELAY_MS);
			}

			HESP8266_vAbort();
			Connect4_vStop();

			/* Check for firmware update between games */
			OTA_vCheckForUpdate();
			continue;
		}

		if (IsMemoryGame(body)) {
			Memory_vStart();
			while (1) {
				if (HESP8266_u8EnsureConnection(HESP8266_WIFI_SSID,
						HESP8266_WIFI_PASSWORD, HESP8266_SERVER_IP,
						HESP8266_SERVER_PORT, CONNECTION_RETRIES)
						!= HESP8266_STATUS_OK) {
					HESP8266_vAbort();
					MSYSTICK_vSetDelay_ms(MEMORY_POLL_DELAY_MS);
					continue;
				}

				if (HESP8266_u8HttpGet(MEMORY_STATUS_PATH, body,
						sizeof(body)) != HESP8266_STATUS_OK) {
					HESP8266_vAbort();
					MSYSTICK_vSetDelay_ms(MEMORY_POLL_DELAY_MS);
					continue;
				}
				HESP8266_vCloseConnection();

				if (!ParseMemoryState(body, &memory_state)) {
					MSYSTICK_vSetDelay_ms(MEMORY_POLL_DELAY_MS);
					continue;
				}
				Memory_vUpdate(&memory_state);
				if (!memory_state.Alive
						|| memory_state.Status == MEMORY_STATUS_ENDED) {
					if (memory_state.Status == MEMORY_STATUS_ENDED) {
						MSYSTICK_vSetDelay_ms(RESULT_DISPLAY_MS);
					}
					break;
				}
				MSYSTICK_vSetDelay_ms(MEMORY_POLL_DELAY_MS);
			}

			HESP8266_vAbort();
			Memory_vStop();

			/* Check for firmware update between games */
			OTA_vCheckForUpdate();
			continue;
		}

		if (IsYatzyGame(body)) {
			Yatzy_vStart();
			while (1) {
				if (HESP8266_u8EnsureConnection(HESP8266_WIFI_SSID,
						HESP8266_WIFI_PASSWORD, HESP8266_SERVER_IP,
						HESP8266_SERVER_PORT, CONNECTION_RETRIES)
						!= HESP8266_STATUS_OK) {
					HESP8266_vAbort();
					MSYSTICK_vSetDelay_ms(YATZY_POLL_DELAY_MS);
					continue;
				}

				if (HESP8266_u8HttpGet(YATZY_STATUS_PATH, body,
						sizeof(body)) != HESP8266_STATUS_OK) {
					HESP8266_vAbort();
					MSYSTICK_vSetDelay_ms(YATZY_POLL_DELAY_MS);
					continue;
				}
				HESP8266_vCloseConnection();

				if (!ParseYatzyState(body, &yatzy_state)) {
					MSYSTICK_vSetDelay_ms(YATZY_POLL_DELAY_MS);
					continue;
				}
				Yatzy_vUpdate(&yatzy_state);
				if (!yatzy_state.Alive
						|| yatzy_state.Status == YATZY_STATUS_ENDED) {
					if (yatzy_state.Status == YATZY_STATUS_ENDED) {
						MSYSTICK_vSetDelay_ms(RESULT_DISPLAY_MS);
					}
					break;
				}
				MSYSTICK_vSetDelay_ms(YATZY_POLL_DELAY_MS);
			}

			HESP8266_vAbort();
			Yatzy_vStop();

			/* Check for firmware update between games */
			OTA_vCheckForUpdate();
			continue;
		}

		if (!IsRpsGame(body)) {
			MSYSTICK_vSetDelay_ms(IDLE_DELAY_MS);
			continue;
		}

		G_xHESP8266DebugStage = HESP8266_DEBUG_RPS_START;
		RPS_vStart();
		while (1) {
			if (HESP8266_u8EnsureConnection(HESP8266_WIFI_SSID,
					HESP8266_WIFI_PASSWORD, HESP8266_SERVER_IP,
					HESP8266_SERVER_PORT, CONNECTION_RETRIES)
					!= HESP8266_STATUS_OK) {
				HESP8266_vAbort();
				MSYSTICK_vSetDelay_ms(RPS_POLL_DELAY_MS);
				continue;
			}

			if (HESP8266_u8HttpGet(RPS_STATUS_PATH, body, sizeof(body))
					!= HESP8266_STATUS_OK) {
				HESP8266_vAbort();
				MSYSTICK_vSetDelay_ms(RPS_POLL_DELAY_MS);
				continue;
			}
			HESP8266_vCloseConnection();

			if (!ParseRpsState(body, &rps_state)) {
				MSYSTICK_vSetDelay_ms(RPS_POLL_DELAY_MS);
				continue;
			}
			RPS_vUpdate(&rps_state);
			if (!rps_state.Alive || rps_state.Status == RPS_STATUS_ENDED) {
				if (rps_state.Status == RPS_STATUS_ENDED) {
					MSYSTICK_vSetDelay_ms(RESULT_DISPLAY_MS);
				}
				break;
			}
			MSYSTICK_vSetDelay_ms(RPS_POLL_DELAY_MS);
		}

		HESP8266_vAbort();
		RPS_vStop();

		/* Check for firmware update between games */
		OTA_vCheckForUpdate();
	}
}

/* Only hardware setup precedes the IR-controlled boot screen. */
int main(void)
{
    InitHardware();
    if (!HIR_u8Init()) {
        MENU_vText(22, 76, "IR CLOCK ERROR", TFT_RED, TFT_BLACK, 1);
        while (1) { } /* No fallback input and no automatic online entry. */
    }
    while (1) {
        if (MENU_enChooseMode() == MENU_ONLINE) {
            HIR_vSuspend();
            HTFT_vFillBackgroundColor(TFT_BLACK);
            RunOnlineMode();
        } else {
            OFFLINE_vRun();
        }
    }
}


