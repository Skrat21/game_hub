/*
 * FONT3x5.c
 *
 * Shared procedural 3x5 bitmap font. Originally part of the RPS animation
 * module, factored out into a common layer so the Math Quiz module (and any
 * future game) can reuse the exact same renderer instead of duplicating it.
 *
 *   Glyph layout: 3 columns x 5 rows. Each byte = one column, bit 0..4 = row.
 *   e.g. glyph[0] is the left column; (glyph[col] >> row) & 1 tests a pixel.
 *
 *  Created on: Sep 16, 2026
 *      Author: AHMED SHERIF
 */

#include "FONT3x5.h"
#include "../../HAL/TFT/TFT_int.h"

/* ------------------------------------------------------------------ */
/* Glyph tables (3 columns each; bit index = row 0..4)                 */
/* ------------------------------------------------------------------ */

static const u8 glyph_blank[3] = { 0x00, 0x00, 0x00 };

/* Letters */
static const u8 glyph_a[3] = { 0x1F, 0x05, 0x1F };
static const u8 glyph_b[3] = { 0x1F, 0x15, 0x1B };
static const u8 glyph_c[3] = { 0x1F, 0x11, 0x11 };
static const u8 glyph_d[3] = { 0x1F, 0x11, 0x0E };
static const u8 glyph_e[3] = { 0x1F, 0x15, 0x11 };
static const u8 glyph_f[3] = { 0x1F, 0x05, 0x01 };
static const u8 glyph_g[3] = { 0x1F, 0x19, 0x11 };
static const u8 glyph_h[3] = { 0x1F, 0x04, 0x1F };
static const u8 glyph_i[3] = { 0x11, 0x1F, 0x11 };
static const u8 glyph_j[3] = { 0x10, 0x11, 0x0F };
static const u8 glyph_k[3] = { 0x1F, 0x0A, 0x15 };
static const u8 glyph_l[3] = { 0x1F, 0x10, 0x10 };
static const u8 glyph_m[3] = { 0x1F, 0x03, 0x1F };
static const u8 glyph_n[3] = { 0x1F, 0x02, 0x1F };
static const u8 glyph_o[3] = { 0x0E, 0x11, 0x0E };
static const u8 glyph_p[3] = { 0x1F, 0x05, 0x07 };
static const u8 glyph_q[3] = { 0x0E, 0x11, 0x1E };
static const u8 glyph_r[3] = { 0x1F, 0x05, 0x1A };
static const u8 glyph_s[3] = { 0x17, 0x15, 0x1D };
static const u8 glyph_t[3] = { 0x01, 0x1F, 0x01 };
static const u8 glyph_u[3] = { 0x1F, 0x10, 0x1F };
static const u8 glyph_v[3] = { 0x07, 0x1F, 0x07 };
static const u8 glyph_w[3] = { 0x1F, 0x0C, 0x1F };
static const u8 glyph_x[3] = { 0x15, 0x0A, 0x15 };
static const u8 glyph_y[3] = { 0x07, 0x1C, 0x07 };
static const u8 glyph_z[3] = { 0x1D, 0x15, 0x17 };

/* Digits */
static const u8 glyph_0[3] = { 0x0E, 0x11, 0x0E };
static const u8 glyph_1[3] = { 0x12, 0x1F, 0x10 };
static const u8 glyph_2[3] = { 0x1D, 0x05, 0x17 };
static const u8 glyph_3[3] = { 0x15, 0x15, 0x1F };
static const u8 glyph_4[3] = { 0x05, 0x07, 0x1F };
static const u8 glyph_5[3] = { 0x1B, 0x15, 0x19 };
static const u8 glyph_6[3] = { 0x1F, 0x1D, 0x15 };
static const u8 glyph_7[3] = { 0x01, 0x01, 0x1F };
static const u8 glyph_8[3] = { 0x1F, 0x15, 0x1F };
static const u8 glyph_9[3] = { 0x1B, 0x15, 0x1F };

/* Symbols / operators */
static const u8 glyph_plus[3]   = { 0x04, 0x0E, 0x04 };
static const u8 glyph_minus[3]  = { 0x04, 0x04, 0x04 };
static const u8 glyph_times[3]  = { 0x0A, 0x04, 0x0A };
static const u8 glyph_slash[3]  = { 0x08, 0x04, 0x02 };
static const u8 glyph_equal[3]  = { 0x0A, 0x0A, 0x0A };
static const u8 glyph_colon[3]  = { 0x00, 0x0A, 0x00 };
static const u8 glyph_dot[3]    = { 0x00, 0x04, 0x00 };
static const u8 glyph_bang[3]   = { 0x00, 0x1F, 0x00 };

const u8 *FONT3x5_pu8GetGlyph(char character)
{
	switch (character) {
	case 'A': return glyph_a;
	case 'B': return glyph_b;
	case 'C': return glyph_c;
	case 'D': return glyph_d;
	case 'E': return glyph_e;
	case 'F': return glyph_f;
	case 'G': return glyph_g;
	case 'H': return glyph_h;
	case 'I': return glyph_i;
	case 'J': return glyph_j;
	case 'K': return glyph_k;
	case 'L': return glyph_l;
	case 'M': return glyph_m;
	case 'N': return glyph_n;
	case 'O': return glyph_o;
	case 'P': return glyph_p;
	case 'Q': return glyph_q;
	case 'R': return glyph_r;
	case 'S': return glyph_s;
	case 'T': return glyph_t;
	case 'U': return glyph_u;
	case 'V': return glyph_v;
	case 'W': return glyph_w;
	case 'X': return glyph_x;
	case 'Y': return glyph_y;
	case 'Z': return glyph_z;

	case '0': return glyph_0;
	case '1': return glyph_1;
	case '2': return glyph_2;
	case '3': return glyph_3;
	case '4': return glyph_4;
	case '5': return glyph_5;
	case '6': return glyph_6;
	case '7': return glyph_7;
	case '8': return glyph_8;
	case '9': return glyph_9;

	case '+': return glyph_plus;
	case '-': return glyph_minus;
	case '*': return glyph_times;
	case '/': return glyph_slash;
	case '=': return glyph_equal;
	case ':': return glyph_colon;
	case '.': return glyph_dot;
	case '!': return glyph_bang;

	default: return glyph_blank;
	}
}

void FONT3x5_vDrawText(const char *text, u16 x, u16 y, u16 color)
{
	while (*text != '\0' && x < 128) {
		const u8 *glyph = FONT3x5_pu8GetGlyph(*text++);
		u8 column;
		u8 row;

		for (column = 0; column < 3; column++) {
			for (row = 0; row < 5; row++) {
				if ((glyph[column] >> row) & 1) {
					HTFT_vDrawPixel(x + column, y + row, color);
				}
			}
		}
		x += 4;
	}
}