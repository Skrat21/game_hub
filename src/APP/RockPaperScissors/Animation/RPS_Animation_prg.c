#include "../../../LIB/STD_TYPES.h"

#include "../../../HAL/TFT/TFT_int.h"
#include "../../../MCAL/SYSTICK/SYSTICK_int.h"

#include "../../../APP/Common/FONT3x5.h"
#include "../../../APP/Common/LOADING_ANIM.h"

#include "RPS_Animation.h"

static void DrawRock(void)
{
    /* =========================================================
     * 1. CORE SILHOUETTE & DEEP SHADOW (DARK GRAY)
     * Defines an asymmetrical crag leaning to the right,
     * with a smaller attached boulder at the bottom-right.
     * ========================================================= */
    HTFT_vSetXPos(45, 80);  HTFT_vSetYPos(40, 55);  HTFT_vFillRectangle(TFT_DARK_GRAY); /* Peak */
    HTFT_vSetXPos(35, 90);  HTFT_vSetYPos(55, 85);  HTFT_vFillRectangle(TFT_DARK_GRAY); /* Main body */
    HTFT_vSetXPos(40, 75);  HTFT_vSetYPos(85, 100); HTFT_vFillRectangle(TFT_DARK_GRAY); /* Base */
    HTFT_vSetXPos(75, 100); HTFT_vSetYPos(75, 95);  HTFT_vFillRectangle(TFT_DARK_GRAY); /* Right cluster */

    /* =========================================================
     * 2. MIDTONE MASS (GRAY)
     * Creates a distinct frontal plane. We leave the entire
     * lower-right corner dark to simulate ambient occlusion.
     * ========================================================= */
    HTFT_vSetXPos(47, 75);  HTFT_vSetYPos(42, 55);  HTFT_vFillRectangle(TFT_GRAY);
    HTFT_vSetXPos(37, 80);  HTFT_vSetYPos(55, 75);  HTFT_vFillRectangle(TFT_GRAY);
    HTFT_vSetXPos(42, 65);  HTFT_vSetYPos(75, 95);  HTFT_vFillRectangle(TFT_GRAY);
    HTFT_vSetXPos(77, 95);  HTFT_vSetYPos(77, 85);  HTFT_vFillRectangle(TFT_GRAY); /* Right cluster top */

    /* =========================================================
     * 3. DIRECT SUNLIGHT (LIGHT GRAY)
     * Focuses heavily on the top and left edges. A distinct
     * horizontal cut creates a "ledge" halfway down.
     * ========================================================= */
    HTFT_vSetXPos(47, 65);  HTFT_vSetYPos(42, 48);  HTFT_vFillRectangle(TFT_LIGHT_GRAY); /* Top flat */
    HTFT_vSetXPos(37, 50);  HTFT_vSetYPos(55, 70);  HTFT_vFillRectangle(TFT_LIGHT_GRAY); /* Left wall */
    HTFT_vSetXPos(45, 60);  HTFT_vSetYPos(65, 70);  HTFT_vFillRectangle(TFT_LIGHT_GRAY); /* Ledge catch */
    HTFT_vSetXPos(77, 85);  HTFT_vSetYPos(77, 80);  HTFT_vFillRectangle(TFT_LIGHT_GRAY); /* Right cluster hit */

    /* =========================================================
     * 4. DITHERING & TEXTURE BLENDING (Mixed)
     * Tiny 2x2 or 3x3 blocks that break up the perfectly straight
     * lines between colors, giving a rough, sandy texture.
     * ========================================================= */
    /* Light bleeding into Midtone */
    HTFT_vSetXPos(52, 55);  HTFT_vSetYPos(55, 58);  HTFT_vFillRectangle(TFT_LIGHT_GRAY);
    HTFT_vSetXPos(62, 65);  HTFT_vSetYPos(48, 51);  HTFT_vFillRectangle(TFT_LIGHT_GRAY);
    HTFT_vSetXPos(67, 70);  HTFT_vSetYPos(60, 63);  HTFT_vFillRectangle(TFT_GRAY); /* Pits in the shadow */

    /* Midtone bleeding into Shadow */
    HTFT_vSetXPos(72, 75);  HTFT_vSetYPos(80, 83);  HTFT_vFillRectangle(TFT_GRAY);
    HTFT_vSetXPos(65, 68);  HTFT_vSetYPos(88, 91);  HTFT_vFillRectangle(TFT_GRAY);

    /* =========================================================
     * 5. EDGE GLARE (WHITE)
     * Placed strictly on the most extreme upper-left corners
     * to pop the contrast and define the sharpest geometry.
     * ========================================================= */
    HTFT_vSetXPos(47, 52);  HTFT_vSetYPos(42, 44);  HTFT_vFillRectangle(TFT_WHITE);
    HTFT_vSetXPos(37, 40);  HTFT_vSetYPos(55, 60);  HTFT_vFillRectangle(TFT_WHITE);
    HTFT_vSetXPos(45, 50);  HTFT_vSetYPos(65, 67);  HTFT_vFillRectangle(TFT_WHITE); /* Ledge glint */

    /* =========================================================
     * 6. FRACTURES (BLACK)
     * Used sparingly to separate the main mass from the side
     * cluster, adding deep structural depth.
     * ========================================================= */
    HTFT_vSetXPos(70, 75);  HTFT_vSetYPos(75, 85);  HTFT_vFillRectangle(TFT_BLACK); /* Crevice */
    HTFT_vSetXPos(66, 70);  HTFT_vSetYPos(85, 95);  HTFT_vFillRectangle(TFT_BLACK);

    /* =========================================================
     * 7. GROUND CONTACT SHADOW (BLACK)
     * Fits the new asymmetrical base shape perfectly.
     * ========================================================= */
    HTFT_vSetXPos(38, 80);  HTFT_vSetYPos(100, 104); HTFT_vFillRectangle(TFT_BLACK);
    HTFT_vSetXPos(75, 102); HTFT_vSetYPos(95, 99);   HTFT_vFillRectangle(TFT_BLACK);
}

/* ============================================================
 * PAPER
 * ============================================================ */
static void DrawPaper(void)
{
    HTFT_vSetXPos(30, 97);
    HTFT_vSetYPos(25, 135);
    HTFT_vFillRectangle(TFT_WHITE);

    HTFT_vSetXPos(26, 30);
    HTFT_vSetYPos(35, 125);
    HTFT_vFillRectangle(TFT_LIGHT_GRAY);

    HTFT_vSetXPos(97, 101);
    HTFT_vSetYPos(35, 125);
    HTFT_vFillRectangle(TFT_LIGHT_GRAY);

    HTFT_vSetXPos(37, 40);
    HTFT_vSetYPos(35, 128);
    HTFT_vFillRectangle(TFT_RED);

    HTFT_vSetXPos(42, 86);
    HTFT_vSetYPos(55, 58);
    HTFT_vFillRectangle(TFT_BLUE);

    HTFT_vSetXPos(42, 86);
    HTFT_vSetYPos(75, 78);
    HTFT_vFillRectangle(TFT_BLUE);

    HTFT_vSetXPos(42, 72);
    HTFT_vSetYPos(95, 98);
    HTFT_vFillRectangle(TFT_BLUE);
}

/* ============================================================
 * SCISSORS
 * ============================================================ */
static void DrawScissors(void)
{
    /* ========================================================
     * Handles
     * ======================================================== */

    /* Outer handles */
    HTFT_vDrawCircle(42, 115, 18, TFT_RED);
    HTFT_vDrawCircle(86, 115, 18, TFT_RED);

    /* Handle centers */
    HTFT_vDrawCircle(42, 115, 9, TFT_BLACK);
    HTFT_vDrawCircle(86, 115, 9, TFT_BLACK);

    /* Handle inner highlights */
    HTFT_vDrawCircle(42, 115, 5, TFT_LIGHT_GRAY);
    HTFT_vDrawCircle(86, 115, 5, TFT_LIGHT_GRAY);

    /* ========================================================
     * Central pivot
     * ======================================================== */

    HTFT_vDrawCircle(64, 91, 7, TFT_DARK_GRAY);
    HTFT_vDrawCircle(64, 91, 3, TFT_LIGHT_GRAY);

    /* ========================================================
     * Left blade
     * ======================================================== */

    /* Main blade */
    HTFT_vSetXPos(56, 63);
    HTFT_vSetYPos(38, 91);
    HTFT_vFillRectangle(TFT_LIGHT_GRAY);

    /* Blade wider section */
    HTFT_vSetXPos(52, 60);
    HTFT_vSetYPos(45, 75);
    HTFT_vFillRectangle(TFT_LIGHT_GRAY);

    /* Blade tip */
    HTFT_vSetXPos(60, 63);
    HTFT_vSetYPos(30, 45);
    HTFT_vFillRectangle(TFT_WHITE);

    /* Blade highlight */
    HTFT_vSetXPos(56, 58);
    HTFT_vSetYPos(40, 78);
    HTFT_vFillRectangle(TFT_WHITE);

    /* ========================================================
     * Right blade
     * ======================================================== */

    HTFT_vSetXPos(65, 72);
    HTFT_vSetYPos(38, 91);
    HTFT_vFillRectangle(TFT_LIGHT_GRAY);

    HTFT_vSetXPos(68, 76);
    HTFT_vSetYPos(45, 75);
    HTFT_vFillRectangle(TFT_LIGHT_GRAY);

    /* Blade tip */
    HTFT_vSetXPos(65, 68);
    HTFT_vSetYPos(30, 45);
    HTFT_vFillRectangle(TFT_WHITE);

    /* Blade highlight */
    HTFT_vSetXPos(69, 71);
    HTFT_vSetYPos(40, 78);
    HTFT_vFillRectangle(TFT_WHITE);

    /* ========================================================
     * Grip connection
     * ======================================================== */

    HTFT_vSetXPos(48, 58);
    HTFT_vSetYPos(90, 101);
    HTFT_vFillRectangle(TFT_RED);

    HTFT_vSetXPos(70, 80);
    HTFT_vSetYPos(90, 101);
    HTFT_vFillRectangle(TFT_RED);

    /* Pivot on top */
    HTFT_vDrawCircle(64, 91, 5, TFT_DARK_GRAY);
    HTFT_vDrawCircle(64, 91, 2, TFT_WHITE);
}

void RPS_Animation_vStart(void)
{
    HTFT_vFillBackgroundColor(TFT_BLACK);
    LOADING_ANIM_vPlay();
}

static void DrawChoiceIcon(RPS_Choice choice, u16 center_x, u16 color)
{
    if (choice == ROCK) {
        HTFT_vFillCircle(center_x, 72, 22, color);
        HTFT_vDrawCircle(center_x, 72, 22, TFT_WHITE);
    } else if (choice == PAPER) {
        HTFT_vSetXPos(center_x - 18, center_x + 18);
        HTFT_vSetYPos(50, 94);
        HTFT_vFillRectangle(color);
        HTFT_vSetXPos(center_x - 18, center_x + 18);
        HTFT_vSetYPos(50, 94);
        HTFT_vDrawLine(center_x - 18, 50, center_x + 18, 50, TFT_WHITE);
        HTFT_vDrawLine(center_x + 18, 50, center_x + 18, 94, TFT_WHITE);
        HTFT_vDrawLine(center_x + 18, 94, center_x - 18, 94, TFT_WHITE);
        HTFT_vDrawLine(center_x - 18, 94, center_x - 18, 50, TFT_WHITE);
    } else if (choice == SCISSORS) {
        HTFT_vDrawCircle(center_x - 10, 84, 10, color);
        HTFT_vDrawCircle(center_x + 10, 84, 10, color);
        HTFT_vDrawLine(center_x - 5, 78, center_x + 18, 48, color);
        HTFT_vDrawLine(center_x + 5, 78, center_x - 18, 48, color);
    } else {
        HTFT_vDrawCircle(center_x, 72, 5, TFT_GRAY);
    }
}

static void DrawDetailedChoice(RPS_Choice choice)
{
    if (choice == ROCK) {
        DrawRock();
    } else if (choice == PAPER) {
        DrawPaper();
    } else if (choice == SCISSORS) {
        DrawScissors();
    }
}

static const char *ChoiceName(RPS_Choice choice)
{
    if (choice == ROCK) return "ROCK";
    if (choice == PAPER) return "PAPER";
    if (choice == SCISSORS) return "SCISSORS";
    return "WAIT";
}

static void DrawResultText(const RPS_ServerState *state)
{
    char player_one[16] = "P1 ";
    char player_two[16] = "P2 ";
    u8 index = 3;
    const char *name;

    name = ChoiceName(state->Player1_Choice);
    while (*name != '\0' && index < sizeof(player_one) - 1) {
        player_one[index++] = *name++;
    }
    player_one[index] = '\0';

    index = 3;
    name = ChoiceName(state->Player2_Choice);
    while (*name != '\0' && index < sizeof(player_two) - 1) {
        player_two[index++] = *name++;
    }
    player_two[index] = '\0';

    if (state->Winner == RPS_WINNER_P1) {
        FONT3x5_vDrawText("P1 WINS", 2, 143, TFT_GREEN);
    } else if (state->Winner == RPS_WINNER_P2) {
        FONT3x5_vDrawText("P2 WINS", 2, 143, TFT_GREEN);
    } else {
        FONT3x5_vDrawText("DRAW", 2, 143, TFT_YELLOW);
    }
    FONT3x5_vDrawText(player_one, 2, 149, TFT_CYAN);
    FONT3x5_vDrawText(player_two, 2, 155, TFT_MAGENTA);
}

void RPS_Animation_vShowState(const RPS_ServerState *state)
{
    if (state == NULL || (!state->Alive && state->Status != RPS_STATUS_ENDED)) {
        return;
    }

    HTFT_vFillBackgroundColor(TFT_BLACK);
    if (state->Status == RPS_STATUS_ENDED) {
        HTFT_vDrawCircle(64, 72, 24, TFT_RED);
        HTFT_vDrawLine(48, 56, 80, 88, TFT_WHITE);
        HTFT_vDrawLine(80, 56, 48, 88, TFT_WHITE);
        return;
    }

    if (state->Status == RPS_STATUS_WAITING) {
        if (state->Player1_Choice != RPS_CHOICE_NONE) {
            DrawDetailedChoice(state->Player1_Choice);
        } else {
            HTFT_vDrawCircle(64, 72, 22, TFT_GRAY);
        }
        return;
    }

    if (state->Status == RPS_STATUS_RESULT) {
        if (state->Winner == RPS_WINNER_P1) {
            DrawDetailedChoice(state->Player1_Choice);
        } else if (state->Winner == RPS_WINNER_P2) {
            DrawDetailedChoice(state->Player2_Choice);
        } else {
            DrawChoiceIcon(state->Player1_Choice, 32, TFT_CYAN);
            DrawChoiceIcon(state->Player2_Choice, 96, TFT_MAGENTA);
        }
        DrawResultText(state);
        return;
    }

    if (state->Player1_Choice == RPS_CHOICE_NONE
            && state->Player2_Choice != RPS_CHOICE_NONE) {
        DrawDetailedChoice(state->Player2_Choice);
    } else if (state->Player2_Choice == RPS_CHOICE_NONE
            && state->Player1_Choice != RPS_CHOICE_NONE) {
        DrawDetailedChoice(state->Player1_Choice);
    } else {
        DrawChoiceIcon(state->Player1_Choice, 32, TFT_CYAN);
        DrawChoiceIcon(state->Player2_Choice, 96, TFT_MAGENTA);
    }
}

void RPS_Animation_vShowIdle(void)
{
    HTFT_vFillBackgroundColor(TFT_BLACK);
    HTFT_vDrawCircle(64, 72, 22, TFT_GRAY);
}

