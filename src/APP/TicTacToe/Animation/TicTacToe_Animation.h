/*
 * TicTacToe_Animation.h
 *
 * Online Tic-Tac-Toe (XO) TFT renderer using shared BOARD_ANIM.
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_TICTACTOE_ANIMATION_TICTACTOE_ANIMATION_H_
#define APP_TICTACTOE_ANIMATION_TICTACTOE_ANIMATION_H_

#include "../game_int.h"

void XO_Animation_vStart(void);
void XO_Animation_vShowState(const XO_ServerState *state);
void XO_Animation_vShowIdle(void);

#endif /* APP_TICTACTOE_ANIMATION_TICTACTOE_ANIMATION_H_ */