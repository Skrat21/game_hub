/*
 * OTA_LED_prg.c
 *
 * Non-blocking OTA status LED on PA11 (see OTA_LED_int.h).
 *
 * The blink is driven by elapsed time from the free-running 1 MHz TIM2
 * counter (HIR_u32NowUS()). No delay loop is used and no SysTick resource is
 * taken, so an OTA download keeps its throughput while the LED blinks and the
 * LED can never stall the update.
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#include "../../LIB/STD_TYPES.h"

#include "../../MCAL/GPIO/GPIO_int.h"
#include "../IR/IR_int.h"

#include "OTA_LED_int.h"

static u8 G_u8Active = 0;      /* 1 while blinking is requested */
static u8 G_u8Level = 0;       /* current pin level while blinking */
static u32 G_u32LastToggleUS = 0;

static void OTA_LED_vWrite(u8 level)
{
    G_u8Level = level;
    MGPIO_vSetPinValueAtomic(OTA_LED_PORT, OTA_LED_PIN,
            level ? GPIO_HIGH : GPIO_LOW);
}

void OTA_LED_vInit(void)
{
    GPIOx_PinConfig_t led = {
        .Port = OTA_LED_PORT,
        .Pin = OTA_LED_PIN,
        .Mode = GPIO_MODE_OUTPUT,
        .OutputType = GPIO_OT_PUSHPULL,
        .Speed = GPIO_SPEED_LOW,
        .PullType = GPIO_NO_PULL,
        .AltFunc = GPIO_AF0
    };

    MGPIO_vPinInit(&led);
    G_u8Active = 0;
    OTA_LED_vWrite(0);
}

void OTA_LED_vOff(void)
{
    G_u8Active = 0;
    OTA_LED_vWrite(0);
}

void OTA_LED_vOn(void)
{
    G_u8Active = 0;
    OTA_LED_vWrite(1);
}

void OTA_LED_vSetActive(u8 A_u8Active)
{
    if (A_u8Active) {
        /* Restart the blink phase on every transition so the LED always
         * begins with a visible "on" segment. */
        G_u32LastToggleUS = HIR_u32NowUS();
        OTA_LED_vWrite(1);
        G_u8Active = 1;
    } else {
        G_u8Active = 0;
        OTA_LED_vWrite(0);
    }
}

void OTA_LED_vTick(void)
{
    u32 now;

    if (!G_u8Active) {
        return;
    }

    now = HIR_u32NowUS();
    /* u32 subtraction is wrap-safe: TIM2->CNT is a 32-bit 1 MHz counter and
     * rolls over roughly every 71 minutes. */
    if ((u32)(now - G_u32LastToggleUS) >= OTA_LED_BLINK_HALF_PERIOD_US) {
        G_u32LastToggleUS = now;
        OTA_LED_vWrite(G_u8Level ? 0 : 1);
    }
}
