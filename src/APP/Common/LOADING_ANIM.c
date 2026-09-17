/*
 * LOADING_ANIM.c
 *
 * Shared loading-spinner playback helper for the TFT display.
 *
 *  Created on: Sep 16, 2026
 *      Author: AHMED SHERIF
 */

#include "LOADING_ANIM.h"
#include "../../HAL/TFT/TFT_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"

void LOADING_ANIM_vPlay(void)
{
	u8 current_frame = 0;
	u8 steps;

	for (steps = 0; steps < LOADING_ANIM_FRAMES; steps++) {
		HTFT_vDrawLoadingSpinner(current_frame);
		current_frame++;
		if (current_frame >= 8) {
			current_frame = 0;
		}
		MSYSTICK_vSetDelay_ms(LOADING_ANIM_FRAME_MS);
	}
}

