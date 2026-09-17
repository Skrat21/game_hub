/*
 * LOADING_ANIM.h
 *
 * Shared loading-spinner playback helper for the TFT display.
 *
 *  Created on: Sep 16, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_COMMON_LOADING_ANIM_H_
#define APP_COMMON_LOADING_ANIM_H_

#include "../../LIB/STD_TYPES.h"

#define LOADING_ANIM_FRAMES    24
#define LOADING_ANIM_FRAME_MS  100

/* Drives HTFT_vDrawLoadingSpinner() through LOADING_ANIM_FRAMES frames of
 * LOADING_ANIM_FRAME_MS each, wrapping the 8-phase frame counter. Shared by
 * every game module so the boot/wait spinner is defined in exactly one place. */
void LOADING_ANIM_vPlay(void);

#endif /* APP_COMMON_LOADING_ANIM_H_ */