/*
 * Connect4_Animation.h
 *
 * Online Connect 4 TFT renderer using shared BOARD_ANIM.
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_CONNECT4_ANIMATION_CONNECT4_ANIMATION_H_
#define APP_CONNECT4_ANIMATION_CONNECT4_ANIMATION_H_

#include "../game_int.h"

void Connect4_Animation_vStart(void);
void Connect4_Animation_vShowState(const Connect4_ServerState *state);
void Connect4_Animation_vShowIdle(void);

#endif /* APP_CONNECT4_ANIMATION_CONNECT4_ANIMATION_H_ */