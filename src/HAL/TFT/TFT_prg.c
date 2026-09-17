#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/SPI/SPI_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"
#include "TFT_int.h"

GPIOx_PinConfig_t TFT_RST_PIN = { .Port = GPIO_PORTA, .Pin = GPIO_PIN0, .Mode =
		GPIO_MODE_OUTPUT, .OutputType = GPIO_SPEED_HIGH, .PullType =
		GPIO_OT_PUSHPULL };

GPIOx_PinConfig_t TFT_A0_PIN = { .Port = GPIO_PORTA, .Pin = GPIO_PIN1, .Mode =
		GPIO_MODE_OUTPUT, .OutputType = GPIO_SPEED_HIGH, .PullType =
		GPIO_OT_PUSHPULL };

volatile static u16 CurrentXStart;
volatile static u16 CurrentYStart;
volatile static u16 CurrentXEnd;
volatile static u16 CurrentYEnd;

static void Reset_Seq(void);
static void Write_cmd(u8 A_u8cmd);
static void Write_data(u8 A_u8cmd);

static void Write_DataBuffer(const u8 *A_u8Data, u32 A_u32Size)
{
    MGPIO_vSetPinValue(TFT_A0_PIN.Port, TFT_A0_PIN.Pin, GPIO_HIGH);

    MSPI_vTransceiveBuffer(A_u8Data, A_u32Size);
}


void HTFT_vInit(void) {
	MGPIO_vPinInit(&TFT_RST_PIN);
	MGPIO_vPinInit(&TFT_A0_PIN);

	MSYSTIC_Config_t cfg = { .InterruptEnable = INT_DISABLE, .CLK_SRC =
			CLK_SRC_AHB_8 };

	MSYSTICK_vInit(&cfg);

	//RST
	Reset_Seq();

	//SLEEP OUT CMD
	Write_cmd(0x11);

	//WAIT 15 msec
	MSYSTICK_vSetDelay_ms(15);

	//Select Mode 3A
	Write_cmd(0X3A);
	Write_data(0x05);

	//Display On
	Write_cmd(0x29);
}

static void DrawHorizontalLine(s16 X0, s16 X1, s16 Y, u16 Color)
{
    s16 Temp;

    /* Check Y */
    if ((Y < 0) || (Y >= 160))
    {
        return;
    }

    /* Make sure X0 <= X1 */
    if (X0 > X1)
    {
        Temp = X0;
        X0 = X1;
        X1 = Temp;
    }

    /* Completely outside screen */
    if ((X1 < 0) || (X0 >= 128))
    {
        return;
    }

    /* Clip left */
    if (X0 < 0)
    {
        X0 = 0;
    }

    /* Clip right */
    if (X1 > 127)
    {
        X1 = 127;
    }

    HTFT_vSetXPos((u16)X0, (u16)X1);
    HTFT_vSetYPos((u16)Y, (u16)Y);
    HTFT_vFillRectangle(Color);
}

static void Write_cmd(u8 A_u8cmd) {
	MGPIO_vSetPinValue(TFT_A0_PIN.Port, TFT_A0_PIN.Pin, GPIO_LOW);
	MSPI_vTransceive(A_u8cmd);

}
static void Write_data(u8 A_u8cmd) {
	MGPIO_vSetPinValue(TFT_A0_PIN.Port, TFT_A0_PIN.Pin, GPIO_HIGH);
	MSPI_vTransceive(A_u8cmd);

}
static void Reset_Seq(void) {
	// RST PIN = 1
	MGPIO_vSetPinValue(TFT_RST_PIN.Port, TFT_RST_PIN.Pin, GPIO_HIGH);
	// DELAY 100 usec
	MSYSTICK_vSetDelay_us(100);
	// RST PIN 0
	MGPIO_vSetPinValue(TFT_RST_PIN.Port, TFT_RST_PIN.Pin, GPIO_LOW);

	// delay 1usec
	MSYSTICK_vSetDelay_us(1);

	// Pin High
	MGPIO_vSetPinValue(TFT_RST_PIN.Port, TFT_RST_PIN.Pin, GPIO_HIGH);

	// delay 100usec
	MSYSTICK_vSetDelay_us(100);

	// pin low
	MGPIO_vSetPinValue(TFT_RST_PIN.Port, TFT_RST_PIN.Pin, GPIO_LOW);

	// delay 100usec
	MSYSTICK_vSetDelay_us(100);

	// pin high
	MGPIO_vSetPinValue(TFT_RST_PIN.Port, TFT_RST_PIN.Pin, GPIO_HIGH);
	// delay 120msec
	MSYSTICK_vSetDelay_ms(120);

}

void HTFT_vDrawPixel(u16 X, u16 Y, u16 Color)
{
    HTFT_vSetXPos(X, X);
    HTFT_vSetYPos(Y, Y);
    HTFT_vFillRectangle(Color);
}

void HTFT_vShowImage(const u16 A_u16ImgArr[], u16 A_u16ImgSize) {
	//set row position
	Write_cmd(0x2A);
	//send xstart 0x0000
	//send msb
	Write_data(0);
	//send lsb
	Write_data(0);
	//send xend 0x007F (127)
	Write_data(0x00);
	Write_data(0x7F);

	//set col position
	Write_cmd(0x2B);
	//send ystart 0x0000
	//send msb
	Write_data(0);
	//send lsb
	Write_data(0);
	//send yend 0x009F (159)
	Write_data(0x00);
	Write_data(0x9F);

	//send image
	Write_cmd(0x2C);
	for (u16 i = 0; i < A_u16ImgSize; i++) {
		u8 MSB = (A_u16ImgArr[i] & 0XFF00) >> 8;
		u8 LSB = (A_u16ImgArr[i] & 0x00FF);
		Write_data(MSB);	// 		send MSB
		Write_data(LSB);	// 	send LSB
	}
}

void HTFT_vSetXPos(u16 A_u16xStart, u16 A_u16xEnd) {
	CurrentXStart = A_u16xStart;
	CurrentXEnd = A_u16xEnd;
	u8 START_MSB = (CurrentXStart & 0XFF00) >> 8;
	u8 START_LSB = (CurrentXStart & 0x00FF);
	u8 END_MSB = (CurrentXEnd & 0XFF00) >> 8;
	u8 END_LSB = (CurrentXEnd & 0x00FF);
	//set row position
	Write_cmd(0x2A);
	//send msb
	Write_data(START_MSB);
	//send lsb
	Write_data(START_LSB);
	//send xend
	Write_data(END_MSB);
	Write_data(END_LSB);
}

void HTFT_vSetYPos(u16 A_u16yStart, u16 A_u16yEnd) {

	CurrentYStart = A_u16yStart;
	CurrentYEnd = A_u16yEnd;
	u8 START_MSB = (A_u16yStart & 0XFF00) >> 8;
	u8 START_LSB = (A_u16yStart & 0x00FF);
	u8 END_MSB = (A_u16yEnd & 0XFF00) >> 8;
	u8 END_LSB = (A_u16yEnd & 0x00FF);
	//set row position
	Write_cmd(0x2B);
	//send msb
	Write_data(START_MSB);
	//send lsb
	Write_data(START_LSB);
	//send xend
	Write_data(END_MSB);
	Write_data(END_LSB);
}

void HTFT_vFillBackgroundColor(u16 A_u16Color) {
	HTFT_vSetXPos(0,127);
	HTFT_vSetYPos(0,159);
	HTFT_vFillRectangle(A_u16Color);
}

void HTFT_vFillRectangle(u16 A_u16Color) {
	u8 MSB = (A_u16Color & 0xFF00) >>8;
	u8 LSB = (A_u16Color & 0x00FF);
    u32 Pixels;

    if (CurrentXEnd < CurrentXStart || CurrentYEnd < CurrentYStart) {
        return;
    }

    Pixels = ((u32) (CurrentXEnd - CurrentXStart + 1)
            * (u32) (CurrentYEnd - CurrentYStart + 1));

	Write_cmd(0x2C);
    for(u32 i=0; i<Pixels;i++)
	{
		Write_data(MSB);
		Write_data(LSB);
	}

}
void HTFT_vDrawLine(u16 A_u16X0, u16 A_u16Y0,
                    u16 A_u16X1, u16 A_u16Y1,
                    u16 A_u16Color)
{
    s16 dx = (s16)A_u16X1 - (s16)A_u16X0;
    s16 dy = (s16)A_u16Y1 - (s16)A_u16Y0;

    s16 sx = (dx >= 0) ? 1 : -1;
    s16 sy = (dy >= 0) ? 1 : -1;

    if (dx < 0)
        dx = -dx;

    if (dy < 0)
        dy = -dy;

    s16 err = dx - dy;

    s16 X = A_u16X0;
    s16 Y = A_u16Y0;

    while (1)
    {
        /* Draw pixel */
        if ((X >= 0) && (X < 128) &&
            (Y >= 0) && (Y < 160))
        {
            HTFT_vDrawPixel((u16)X, (u16)Y, A_u16Color);
        }

        /* Stop at end point */
        if ((X == (s16)A_u16X1) &&
            (Y == (s16)A_u16Y1))
        {
            break;
        }

        s16 e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            X += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            Y += sy;
        }
    }
}

void HTFT_vDrawCircle(u16 A_u16CenterX,
                      u16 A_u16CenterY,
                      u16 A_uRadius,
                      u16 A_u16Color)
{
    s16 x = 0;
    s16 y = A_uRadius;
    s16 d = 1 - (s16)A_uRadius;

    while (x <= y)
    {
        /* Point 1 */
        if ((A_u16CenterX + x < 128) &&
            (A_u16CenterY + y < 160))
            HTFT_vDrawPixel(A_u16CenterX + x,
                            A_u16CenterY + y,
                            A_u16Color);

        /* Point 2 */
        if ((A_u16CenterX + y < 128) &&
            (A_u16CenterY + x < 160))
            HTFT_vDrawPixel(A_u16CenterX + y,
                            A_u16CenterY + x,
                            A_u16Color);

        /* Point 3 */
        if ((A_u16CenterX >= x) &&
            (A_u16CenterY + y < 160))
            HTFT_vDrawPixel(A_u16CenterX - x,
                            A_u16CenterY + y,
                            A_u16Color);

        /* Point 4 */
        if ((A_u16CenterX >= y) &&
            (A_u16CenterY + x < 160))
            HTFT_vDrawPixel(A_u16CenterX - y,
                            A_u16CenterY + x,
                            A_u16Color);

        /* Point 5 */
        if ((A_u16CenterX >= x) &&
            (A_u16CenterY >= y))
            HTFT_vDrawPixel(A_u16CenterX - x,
                            A_u16CenterY - y,
                            A_u16Color);

        /* Point 6 */
        if ((A_u16CenterX >= y) &&
            (A_u16CenterY >= x))
            HTFT_vDrawPixel(A_u16CenterX - y,
                            A_u16CenterY - x,
                            A_u16Color);

        /* Point 7 */
        if ((A_u16CenterX + x < 128) &&
            (A_u16CenterY >= y))
            HTFT_vDrawPixel(A_u16CenterX + x,
                            A_u16CenterY - y,
                            A_u16Color);

        /* Point 8 */
        if ((A_u16CenterX + y < 128) &&
            (A_u16CenterY >= x))
            HTFT_vDrawPixel(A_u16CenterX + y,
                            A_u16CenterY - x,
                            A_u16Color);

        /* Midpoint Circle Algorithm */
        if (d < 0)
        {
            d += (2 * x) + 3;
        }
        else
        {
            d += (2 * (x - y)) + 5;
            y--;
        }

        x++;
    }
}

void HTFT_vFillCircle(u16 A_u16CenterX,
                      u16 A_u16CenterY,
                      u16 A_u16Radius,
                      u16 A_u16Color)
{
    s16 X = 0;
    s16 Y = (s16)A_u16Radius;
    s16 D = 1 - (s16)A_u16Radius;

    while (X <= Y)
    {
        /*
         * Four horizontal lines are enough
         * because of circle symmetry.
         */

        DrawHorizontalLine(
            (s16)A_u16CenterX - Y,
            (s16)A_u16CenterX + Y,
            (s16)A_u16CenterY + X,
            A_u16Color
        );

        DrawHorizontalLine(
            (s16)A_u16CenterX - Y,
            (s16)A_u16CenterX + Y,
            (s16)A_u16CenterY - X,
            A_u16Color
        );

        DrawHorizontalLine(
            (s16)A_u16CenterX - X,
            (s16)A_u16CenterX + X,
            (s16)A_u16CenterY + Y,
            A_u16Color
        );

        DrawHorizontalLine(
            (s16)A_u16CenterX - X,
            (s16)A_u16CenterX + X,
            (s16)A_u16CenterY - Y,
            A_u16Color
        );

        /* Midpoint circle algorithm */

        if (D < 0)
        {
            D += (2 * X) + 3;
        }
        else
        {
            D += (2 * (X - Y)) + 5;
            Y--;
        }

        X++;
    }
}

/* ============================================================
 * LOADING SPINNER (Fading Circular Trail)
 * ============================================================ */

#define SPINNER_DOTS 8

/* Pre-calculated coordinates for a circle with radius 20, centered at (64, 64) */
static const u8 spinner_x[SPINNER_DOTS] = {64, 78, 84, 78, 64, 48, 44, 48};
static const u8 spinner_y[SPINNER_DOTS] = {44, 48, 64, 78, 84, 78, 64, 48};

/* Colors arranged from brightest (the lead dot) to dark (the tail) */
static const u16 trail_colors[SPINNER_DOTS] = {
    TFT_WHITE,        /* Lead dot (head) */
    TFT_LIGHT_GRAY,   /* Trail 1 */
    TFT_GRAY,         /* Trail 2 */
    TFT_DARK_GRAY,    /* Trail 3 */
    TFT_BLACK,        /* Cleared (background) */
    TFT_BLACK,        /* Cleared (background) */
    TFT_BLACK,        /* Cleared (background) */
    TFT_BLACK         /* Cleared (background) */
};

/*
 * Call this function repeatedly in a loop.
 * 'frame' should increment from 0 to 7, then wrap back to 0.
 */
void HTFT_vDrawLoadingSpinner(u8 frame)
{
    /* Ensure frame is safely bounded between 0 and 7 */
    frame = frame % SPINNER_DOTS;

    for (u8 i = 0; i < SPINNER_DOTS; i++)
    {
        /* Calculate how "old" this dot is relative to the current frame.
         * The math maps the head of the trail to color index 0, and the tail to 7. */
        u8 age = (frame + SPINNER_DOTS - i) % SPINNER_DOTS;

        u16 color = trail_colors[age];

        /* Draw a small circle of radius 4 for each dot */
        HTFT_vFillCircle(spinner_x[i], spinner_y[i], 4, color);
    }
}



#define TFT_TX_BUFFER_SIZE    (128 * 20 * 2)

static u8 TFT_TxBuffer[TFT_TX_BUFFER_SIZE];

void HTFT_vDrawBitmap(u16 A_u16XStart,
                      u16 A_u16YStart,
                      u16 A_u16XEnd,
                      u16 A_u16YEnd,
                      const u16 *A_u16Data)
{
    u16 Width;
    u16 Height;
    u32 Pixels;
    u32 Bytes;

    /* Calculate bitmap dimensions */
    Width  = A_u16XEnd - A_u16XStart + 1;
    Height = A_u16YEnd - A_u16YStart + 1;

    Pixels = (u32)Width * Height;
    Bytes  = Pixels * 2;

    /* Make sure bitmap fits inside our temporary buffer */
    if (Bytes > TFT_TX_BUFFER_SIZE)
    {
        return;
    }

    /* Set TFT drawing window */
    HTFT_vSetXPos(A_u16XStart, A_u16XEnd);
    HTFT_vSetYPos(A_u16YStart, A_u16YEnd);

    /* Memory Write command */
    Write_cmd(0x2C);

    /*
     * Convert RGB565 pixels from u16 to
     * the byte order required by the TFT:
     *
     * Pixel = 0xRRGG
     *
     * SPI sends:
     *      MSB
     *      LSB
     */
    for (u32 i = 0; i < Pixels; i++)
    {
        TFT_TxBuffer[2 * i] =
                (u8)((A_u16Data[i] >> 8) & 0xFF);

        TFT_TxBuffer[(2 * i) + 1] =
                (u8)(A_u16Data[i] & 0xFF);
    }

    /* Send all pixel data continuously */
    Write_DataBuffer(TFT_TxBuffer, Bytes);
}
