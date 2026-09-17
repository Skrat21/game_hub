/*
 * OTA_LED_int.h
 *
 * Status LED that blinks (non-blocking) while an OTA firmware update is in
 * progress, and is off otherwise.
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#ifndef HAL_OTA_LED_OTA_LED_INT_H_
#define HAL_OTA_LED_OTA_LED_INT_H_

#include "../../LIB/STD_TYPES.h"

/*
 * LED pin.
 *
 * NOTE: PA10 is USART1_RX (the ESP8266 link, see InitHardware() in main.c),
 * so it cannot be used as the OTA LED. PA11 is free on this board and is used
 * instead. Changing the pin is a one-line edit here.
 */
#define OTA_LED_PORT  GPIO_PORTA
#define OTA_LED_PIN   GPIO_PIN11

/* Half-period of the blink, in microseconds (250 ms -> 2 Hz blink). Kept in
 * the same free-running TIM2 domain as the IR driver's HIR_u32NowUS(), which
 * keeps counting while HIR_vSuspend() only masks the EXTI line. */
#define OTA_LED_BLINK_HALF_PERIOD_US 250000u

/* Configures the pin as a push-pull output, initially off. */
void OTA_LED_vInit(void);

/* Turns the LED off and clears the blink state (safe to call repeatedly). */
void OTA_LED_vOff(void);

/* Turns the LED on steadily and clears the blink state. */
void OTA_LED_vOn(void);

/* Starts / stops the non-blocking blink. Idempotent. */
void OTA_LED_vSetActive(u8 A_u8Active);

/* Non-blocking blink service. Call often (download callback / status redraws
 * / periodic tick). Returns immediately when inactive; never delays. */
void OTA_LED_vTick(void);

#endif /* HAL_OTA_LED_OTA_LED_INT_H_ */
