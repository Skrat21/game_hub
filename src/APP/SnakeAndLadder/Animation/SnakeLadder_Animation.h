/*
 * SnakeLadder_Animation.h
 *
 * Snake and Ladder TFT renderer and animation interface.
 *
 *  Created on: Sep 16, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_SNAKEANDLADDER_ANIMATION_SNAKELADDER_ANIMATION_H_
#define APP_SNAKEANDLADDER_ANIMATION_SNAKELADDER_ANIMATION_H_

#include "../game_int.h"

void SnakeLadder_Animation_vStart(void);
void SnakeLadder_Animation_vShowState(const SnakeLadder_ServerState *state);
void SnakeLadder_Animation_vShowIdle(void);

#endif /* APP_SNAKEANDLADDER_ANIMATION_SNAKELADDER_ANIMATION_H_ */

