#ifndef HAL_IR_IR_INT_H_
#define HAL_IR_IR_INT_H_

#include "../../LIB/STD_TYPES.h"

/* Command bytes/aliases of the reference remote, standard NEC address 0. */
#define HIR_NEC_REMOTE_ADDRESS 0x00
#define HIR_CMD_POWER 0x45
#define HIR_CMD_VOLUME_UP 0x46
#define HIR_CMD_MODE 0x46
#define HIR_CMD_FUNCTION_STOP 0x47
#define HIR_CMD_REWIND 0x40
#define HIR_CMD_PLAY_PAUSE 0x40
#define HIR_CMD_FAST_FORWARD 0x43
#define HIR_CMD_DOWN 0x07
#define HIR_CMD_EQ 0x07
#define HIR_CMD_VOLUME_DOWN 0x15
#define HIR_CMD_UP 0x09
#define HIR_CMD_0 0x16
#define HIR_CMD_ST_REPT 0x0D
#define HIR_CMD_1 0x0C
#define HIR_CMD_2 0x18
#define HIR_CMD_3 0x5E
#define HIR_CMD_4 0x08
#define HIR_CMD_5 0x1C
#define HIR_CMD_6 0x5A
#define HIR_CMD_7 0x42
#define HIR_CMD_8 0x52
#define HIR_CMD_9 0x4A

typedef struct {
    u8 Address;
    u8 Command;
} HIR_Command_t;

/* Call after system clock setup. Owns TIM2 and PA6/EXTI6; returns 0 if the
 * timer clock cannot provide an exact 1 us tick. No UART or SysTick usage. */
u8 HIR_u8Init(void);
/* Single foreground consumer; returns one validated frame, or 0 if empty.
 * The consumer filters Address (the menu/games accept address 0 only). */
u8 HIR_u8Poll(HIR_Command_t *command);
void HIR_vFlush(void);
/* Stop IR interrupts before entering the unchanged online flow. */
void HIR_vSuspend(void);
u32 HIR_u32NowUS(void);

/* Debugger-visible diagnostics; no traffic is sent to the ESP8266 UART. */
extern volatile u32 G_u32HIREdges;
extern volatile u32 G_u32HIRFrames;
extern volatile u32 G_u32HIROverflows;
#endif
