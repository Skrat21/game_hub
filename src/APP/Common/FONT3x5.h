/*
 * FONT3x5.h
 *
 * Shared 3x5 pixel font engine for the TFT display.
 *
 *  Created on: Sep 16, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_COMMON_FONT3X5_H_
#define APP_COMMON_FONT3X5_H_

#include "../../LIB/STD_TYPES.h"

/* Returns a 3-column x 5-row bitmap for `character`, laid out as an array of
 * 3 bytes. Each byte is one column; bit `row` (0..4) is the pixel offset.
 * Unsupported characters return a blank glyph. */
const u8 *FONT3x5_pu8GetGlyph(char character);

/* Draws a string of 3x5 glyphs starting at top-left (x, y). Each character
 * advances the cursor by 4 pixels (3 wide + 1 gap). Pixels are drawn with
 * the given TFT colour. */
void FONT3x5_vDrawText(const char *text, u16 x, u16 y, u16 color);

#endif /* APP_COMMON_FONT3X5_H_ */