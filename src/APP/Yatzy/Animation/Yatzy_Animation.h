/*
 * Yatzy_Animation.h
 *
 * Online Yatzy TFT renderer using BOARD_ANIM.
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_YATZY_ANIMATION_YATZY_ANIMATION_H_
#define APP_YATZY_ANIMATION_YATZY_ANIMATION_H_

#include "../game_int.h"

void Yatzy_Animation_vStart(void);
void Yatzy_Animation_vShowState(const Yatzy_ServerState *state);
void Yatzy_Animation_vShowIdle(void);

#endif /* APP_YATZY_ANIMATION_YATZY_ANIMATION_H_ */