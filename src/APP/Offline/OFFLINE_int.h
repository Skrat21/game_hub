#ifndef APP_OFFLINE_INT_H_
#define APP_OFFLINE_INT_H_
#include "../../LIB/STD_TYPES.h"
void OFFLINE_vRun(void);
void OFFLINE_vConnect4(void);
void OFFLINE_vTicTacToe(void);
void OFFLINE_vSnake(void);
/* Foreground input only; filters remote address. */
u8 OFFLINE_u8Command(u8 *command);
/* Result dwell: consume commands, preserve Power/exit, discard other input. */
u8 OFFLINE_u8Wait(u32 duration_us);
/* Shared board rule used by the two turn-based games. */
u8 OFFLINE_u8Winner(const u8 *board, u8 rows, u8 cols, u8 run, u8 player);
#endif
