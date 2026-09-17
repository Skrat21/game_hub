/*
 * TFT_int.h
 *
 *  Created on: Sep 3, 2026
 *      Author: AHMED SHERIF
 */

#ifndef HAL_TFT_TFT_INT_H_
#define HAL_TFT_TFT_INT_H_

#define TFT_BLACK       0x0000
#define TFT_WHITE       0xFFFF

#define TFT_RED         0xF800
#define TFT_GREEN       0x07E0
#define TFT_BLUE        0x001F

#define TFT_YELLOW      0xFFE0
#define TFT_CYAN        0x07FF
#define TFT_MAGENTA     0xF81F

#define TFT_ORANGE      0xFD20
#define TFT_PURPLE      0x780F
#define TFT_PINK        0xF81F

#define TFT_LIGHT_GRAY  0xC618
#define TFT_GRAY        0x8410
#define TFT_DARK_GRAY   0x4208

#define TFT_BROWN       0xA145
#define TFT_LIME        0x07E0

void HTFT_vInit(void);
void HTFT_vShowImage(const u16 A_u16ImgArr[], u16 A_u16ImgSize);
void HTFT_vSetXPos(u16 A_u16xStart, u16 A_u16xEnd);
void HTFT_vSetYPos(u16 A_u16yStart, u16 A_u16yEnd);
void HTFT_vDrawPixel(u16 X, u16 Y, u16 Color);
void HTFT_vFillBackgroundColor(u16 A_u16Color);
void HTFT_vFillRectangle(u16 A_u16Color);
void HTFT_vDrawLine(u16 A_u16X0, u16 A_u16Y0, u16 A_uX1, u16 A_uY1,
		u16 A_u16Color);
void HTFT_vDrawCircle(u16 A_u16CenterX, u16 A_u16CenterY, u16 A_uRadius,
		u16 A_u16Color);
void HTFT_vFillCircle(u16 A_u16CenterX, u16 A_u16CenterY, u16 A_u16Radius,
		u16 A_u16Color);
void HTFT_vDrawLoadingSpinner(u8 frame);
void HTFT_vDrawBitmap(u16 A_u16XStart,
                      u16 A_u16YStart,
                      u16 A_u16XEnd,
                      u16 A_u16YEnd,
                      const u16 *A_u16Data);

#endif /* HAL_TFT_TFT_INT_H_ */
