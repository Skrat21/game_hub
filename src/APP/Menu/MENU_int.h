#ifndef APP_MENU_INT_H_
#define APP_MENU_INT_H_
#include "../../LIB/STD_TYPES.h"
typedef enum { MENU_ONLINE = 0, MENU_OFFLINE = 1 } MENU_Mode_t;
MENU_Mode_t MENU_enChooseMode(void);
/* Shared reference navigation: previous 40, next 43, select 07.
 * Returns 1 for select, 2 for Mode/back, otherwise 0. */
u8 MENU_u8Navigate(u8 command, u8 count, u8 *selection);
#endif
