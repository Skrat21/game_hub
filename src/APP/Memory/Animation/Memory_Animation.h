/*
 * Memory_Animation.h
 *
 * Online Memory matching game TFT renderer using BOARD_ANIM.
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_MEMORY_ANIMATION_MEMORY_ANIMATION_H_
#define APP_MEMORY_ANIMATION_MEMORY_ANIMATION_H_

#include "../game_int.h"

void Memory_Animation_vStart(void);
void Memory_Animation_vShowState(const Memory_ServerState *state);
void Memory_Animation_vShowIdle(void);

#endif /* APP_MEMORY_ANIMATION_MEMORY_ANIMATION_H_ */