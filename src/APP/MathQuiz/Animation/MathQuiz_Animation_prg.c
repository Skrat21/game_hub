#include "../../../LIB/STD_TYPES.h"

#include "../../../HAL/TFT/TFT_int.h"
#include "../../../MCAL/SYSTICK/SYSTICK_int.h"

#include "../../../APP/Common/FONT3x5.h"
#include "../../../APP/Common/LOADING_ANIM.h"

#include "MathQuiz_Animation.h"

#define FONT_SCALE     3
#define TEXT_ROW       104

static u8 G_u8P1Score = 0;
static u8 G_u8P2Score = 0;
static u8 G_u8LastCountedRound = 0;

static u16 TextLength(const char *text)
{
	u16 length = 0;
	while (*text != '\0') {
		length++;
		text++;
	}
	return length;
}

static void AppendNumber(char *text, u8 *length, u8 value)
{
	if (value >= 100) {
		text[(*length)++] = '0' + (value / 100);
	}
	if (value >= 10) {
		text[(*length)++] = '0' + ((value / 10) % 10);
	}
	text[(*length)++] = '0' + (value % 10);
}

static void DrawTextCentered(const char *text, u16 y, u16 color)
{
	u16 width = (u16) (TextLength(text) * 4);
	u16 x = width >= 128 ? 0 : (u16) ((128 - width) >> 1);
	FONT3x5_vDrawText(text, x, y, color);
}

static void DrawScaledText(const char *text, u16 x, u16 y, u16 scale, u16 color)
{
	while (*text != '\0' && x < 120) {
		const u8 *glyph = FONT3x5_pu8GetGlyph(*text++);
		u8 column;
		u8 row;

		for (column = 0; column < 3; column++) {
			for (row = 0; row < 5; row++) {
				if ((glyph[column] >> row) & 1) {
					u16 dx;
					u16 dy;
					for (dx = 0; dx < scale; dx++) {
						for (dy = 0; dy < scale; dy++) {
							HTFT_vDrawPixel(x + column * scale + dx,
									y + row * scale + dy, color);
						}
					}
				}
			}
		}
		x = (u16) (x + 4 * scale);
	}
}

static void DrawScaledCentered(const char *text, u16 y, u16 scale, u16 color)
{
	u16 width = (u16) (TextLength(text) * 4 * scale);
	u16 x = width >= 128 ? 0 : (u16) ((128 - width) >> 1);
	DrawScaledText(text, x, y, scale, color);
}

/* Correct indicator: solid green filled disc. */
static void DrawCorrectMark(u16 x, u16 y)
{
	HTFT_vFillCircle((u16) (x + 3), (u16) (y + 3), 3, TFT_GREEN);
	HTFT_vDrawCircle((u16) (x + 3), (u16) (y + 3), 3, TFT_GREEN);
}

/* Incorrect indicator: red cross (X). */
static void DrawWrongMark(u16 x, u16 y)
{
	HTFT_vDrawLine(x, y, (u16) (x + 6), (u16) (y + 6), TFT_RED);
	HTFT_vDrawLine((u16) (x + 6), y, x, (u16) (y + 6), TFT_RED);
}

static void UpdateScores(const MathQuiz_ServerState *state)
{
	if (state->Status == MQ_STATUS_RESULT
			&& state->Winner != MQ_WINNER_NONE
			&& state->Round != G_u8LastCountedRound) {
		G_u8LastCountedRound = state->Round;
		if (state->Winner == MQ_WINNER_P1) {
			G_u8P1Score++;
		} else if (state->Winner == MQ_WINNER_P2) {
			G_u8P2Score++;
		}
	}
}

static void DrawScoreRow(u16 color)
{
	char line[16];
	u8 length = 0;

	line[length++] = 'P';
	line[length++] = '1';
	line[length++] = ' ';
	AppendNumber(line, &length, G_u8P1Score);
	line[length++] = ' ';
	line[length++] = ':';
	line[length++] = ' ';
	line[length++] = 'P';
	line[length++] = '2';
	line[length++] = ' ';
	AppendNumber(line, &length, G_u8P2Score);
	line[length] = '\0';

	DrawTextCentered(line, 2, color);
}

static void DrawRoundRow(const MathQuiz_ServerState *state)
{
	char line[12];
	u8 length = 0;

	line[length++] = 'R';
	line[length++] = 'N';
	line[length++] = 'D';
	line[length++] = ' ';
	AppendNumber(line, &length, state->Round);
	line[length] = '\0';

	DrawTextCentered(line, 12, TFT_YELLOW);
}

static void DrawAnswer(u16 label_x, u16 label_color, const char *label,
		const char *value, u8 answered, u8 result, u8 correct)
{
	u16 value_x = (u16) (label_x + TextLength(label) * 4 + 3);

	FONT3x5_vDrawText(label, label_x, TEXT_ROW, label_color);
	if (!answered) {
		FONT3x5_vDrawText("--", value_x, TEXT_ROW, TFT_GRAY);
		return;
	}
	FONT3x5_vDrawText(value, value_x, TEXT_ROW, TFT_WHITE);
	if (result) {
		u16 mark_x = (u16) (value_x + TextLength(value) * 4 + 2);
		if (correct) {
			DrawCorrectMark(mark_x, (u16) (TEXT_ROW + 1));
		} else {
			DrawWrongMark(mark_x, (u16) (TEXT_ROW + 1));
		}
	}
}

static void DrawPlayers(const MathQuiz_ServerState *state)
{
	u8 result = (state->Status == MQ_STATUS_RESULT);
	u8 p1_correct = (state->Winner == MQ_WINNER_P1);
	u8 p2_correct = (state->Winner == MQ_WINNER_P2);

	DrawAnswer(4, TFT_CYAN, "P1", state->Player1_Answer,
			state->Player1_AnswerIsSet, result, p1_correct);
	DrawAnswer(80, TFT_MAGENTA, "P2", state->Player2_Answer,
			state->Player2_AnswerIsSet, result, p2_correct);
}

static void DrawBanner(const MathQuiz_ServerState *state)
{
	if (state->Status == MQ_STATUS_ENDED) {
		DrawTextCentered("MATCH OVER", 140, TFT_RED);
	} else if (state->Status == MQ_STATUS_RESULT) {
		if (state->Winner == MQ_WINNER_P1) {
			DrawTextCentered("P1 WINS", 140, TFT_GREEN);
		} else if (state->Winner == MQ_WINNER_P2) {
			DrawTextCentered("P2 WINS", 140, TFT_GREEN);
		} else {
			DrawTextCentered("DRAW", 140, TFT_YELLOW);
		}
	} else if (state->Status == MQ_STATUS_PLAYING) {
		DrawTextCentered("ANSWER!", 140, TFT_YELLOW);
	} else {
		DrawTextCentered("WAITING", 140, TFT_GRAY);
	}
}

void MathQuiz_Animation_vStart(void)
{
	G_u8P1Score = 0;
	G_u8P2Score = 0;
	G_u8LastCountedRound = 0;
	HTFT_vFillBackgroundColor(TFT_BLACK);
	LOADING_ANIM_vPlay();
}

void MathQuiz_Animation_vShowState(const MathQuiz_ServerState *state)
{
	if (state == NULL || (!state->Alive && state->Status != MQ_STATUS_ENDED)) {
		return;
	}

	UpdateScores(state);

	HTFT_vFillBackgroundColor(TFT_BLACK);
	DrawScoreRow(TFT_CYAN);
	DrawRoundRow(state);

	if (state->Status == MQ_STATUS_WAITING) {
		DrawScaledCentered("WAIT", 36, FONT_SCALE, TFT_GRAY);
	} else if (state->Question[0] != '\0') {
		DrawScaledCentered(state->Question, 36, FONT_SCALE, TFT_WHITE);
	}

	DrawPlayers(state);
	DrawBanner(state);
}

void MathQuiz_Animation_vShowIdle(void)
{
	HTFT_vFillBackgroundColor(TFT_BLACK);
	DrawTextCentered("READY", 70, TFT_GRAY);
}