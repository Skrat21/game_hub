#ifndef APP_MENU_DRAW_H_
#define APP_MENU_DRAW_H_
#include "../../LIB/STD_TYPES.h"
#include "../../HAL/TFT/TFT_int.h"
void MENU_vRect(u16 x, u16 y, u16 width, u16 height, u16 color);
void MENU_vText(u16 x, u16 y, const char *text, u16 color, u16 background, u8 scale);
void MENU_vGrid(u16 x, u16 y, u16 width, u16 height, u8 cols, u8 rows);
void MENU_vList(const char *const *items, u8 count, u8 selection);
void MENU_vItem(const char *text, u8 index, u8 highlighted);
#endif
