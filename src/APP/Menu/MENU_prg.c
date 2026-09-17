#include "MENU_int.h"
#include "MENU_draw.h"
#include "../../HAL/IR/IR_int.h"

u8 MENU_u8Navigate(u8 command, u8 count, u8 *selection)
{
    if (!count || !selection) return 0;
    switch (command) {
    case HIR_CMD_REWIND:
        *selection = (*selection == 0) ? count - 1 : *selection - 1;
        break;
    case HIR_CMD_FAST_FORWARD:
        *selection = (*selection + 1) % count;
        break;
    case HIR_CMD_EQ: return 1;
    case HIR_CMD_MODE: return 2;
    default: break;
    }
    return 0;
}

MENU_Mode_t MENU_enChooseMode(void)
{
    static const char *const items[] = { "ONLINE MODE", "OFFLINE MODE" };
    u8 selection = 0;
    HIR_Command_t command;
    HIR_vFlush();
    MENU_vList(items, 2, selection);
    while (1) {
        u8 previous = selection;
        if (!HIR_u8Poll(&command) || command.Address != HIR_NEC_REMOTE_ADDRESS)
            continue;
        if (MENU_u8Navigate(command.Command, 2, &selection) == 1) {
            HIR_vFlush();
            return (MENU_Mode_t)selection;
        }
        if (selection != previous) {
            MENU_vItem(items[previous], previous, 0);
            MENU_vItem(items[selection], selection, 1);
        }
    }
}
