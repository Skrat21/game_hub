#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "../../src/HAL/IR/IR_int.h"
#include "../../src/APP/Menu/MENU_int.h"
#include "../../src/APP/Offline/OFFLINE_int.h"
TestTimer test_timer;
TestRcc test_rcc;
TestGpio test_gpio;
TestSyscfg test_syscfg;
TestExti test_exti;
uint32_t SystemCoreClock = 25000000;
static void (*callback)(void);
void SystemCoreClockUpdate(void) { }
void NVIC_SetPriority(int irq, unsigned priority) { (void)irq; assert(priority == 1); }
void NVIC_ClearPendingIRQ(int irq) { (void)irq; }
void NVIC_EnableIRQ(int irq) { (void)irq; }
void MEXTI_vSetCallBack(void (*fn)(void), u8 line) { assert(line == 6); callback = fn; }
/* 128x160 framebuffer recording what the M4 TFT driver would show. */
static u16 fb[160][128];
static u16 wx0, wx1, wy0, wy1;
void HTFT_vSetXPos(u16 x0, u16 x1) { wx0 = x0; wx1 = x1; }
void HTFT_vSetYPos(u16 y0, u16 y1) { wy0 = y0; wy1 = y1; }
void HTFT_vFillRectangle(u16 color)
{
    u16 x, y;
    for (y = wy0; y <= wy1 && y < 160; y++)
        for (x = wx0; x <= wx1 && x < 128; x++) fb[y][x] = color;
}
void HTFT_vFillBackgroundColor(u16 color)
{
    HTFT_vSetXPos(0, 127);
    HTFT_vSetYPos(0, 159);
    HTFT_vFillRectangle(color);
}
static void frame(u8 code, u8 address)
{
    u32 bits = ((u32)address << 24) | ((u32)(address ^ 255) << 16) |
               ((u32)code << 8) | (u32)(code ^ 255);
    unsigned bit;
    edge(50000); edge(13500);
    for (bit = 0; bit < 32; bit++) edge(bits & ((u32)1 << bit) ? 2250 : 1125);
}
static void edge(u32 delta) { test_timer.CNT += delta; callback(); }
static void press(u8 code) { frame(code, 0); }
static int pixel(u16 x, u16 y) { return fb[y][x]; }
int main(void)
{
    test_rcc.CFGR = 5;
    assert(HIR_u8Init());
    /* Boot menu draws both options; default selection is Online. */
    MENU_vList((const char *const[]){"ONLINE MODE", "OFFLINE MODE"}, 2, 0);
    assert(pixel(8, 130) == TFT_CYAN && pixel(8, 115) == TFT_BLACK);
    /* Remote navigation wraps; EQ selects the highlighted mode. */
    press(HIR_CMD_FAST_FORWARD);
    press(HIR_CMD_REWIND);
    press(HIR_CMD_REWIND);
    assert(MENU_enChooseMode() == MENU_OFFLINE); /* wrap to index 1 */
    HIR_vFlush();
    press(HIR_CMD_FAST_FORWARD);
    press(HIR_CMD_EQ);
    assert(MENU_enChooseMode() == MENU_ONLINE);
    HIR_vFlush();
    press(HIR_CMD_EQ);
    press(HIR_CMD_MODE);
    assert(MENU_u8Navigate(HIR_CMD_EQ, 3, &(u8){0}) == 1);
    assert(MENU_u8Navigate(HIR_CMD_MODE, 3, &(u8){0}) == 2);
    /* Offline hub: suppressed confirm, launch Connect 4, exit via Power. */
    press(HIR_CMD_EQ);
    press(HIR_CMD_4);
    press(HIR_CMD_EQ); /* releases suppression and selects index 0 */
    OFFLINE_vRun();
    assert(pixel(15, 26) == TFT_WHITE && pixel(21, 20) == TFT_RED);
    press(HIR_CMD_6);
    press(HIR_CMD_5);
    assert(pixel(31, 46) == TFT_RED);
    assert(pixel(49, 42) == TFT_GREEN); /* head at column 6, row 5 */
    press(HIR_CMD_POWER);
    OFFLINE_vConnect4();
    assert(pixel(8, 115) == TFT_CYAN); /* hub menu redrawn on return */
    /* Tic-tac-toe: X wins on the diagonal, then Power exits. */
    press(HIR_CMD_FAST_FORWARD);
    press(HIR_CMD_EQ);
    OFFLINE_vRun();
    press(HIR_CMD_5);
    press(HIR_CMD_8); press(HIR_CMD_6); press(HIR_CMD_5);
    press(HIR_CMD_2); press(HIR_CMD_4); press(HIR_CMD_5);
    assert(pixel(64, 79) == TFT_RED);
    press(HIR_CMD_POWER);
    OFFLINE_vTicTacToe();
    /* Snake: queue a direction and exit; movement is timing driven. */
    press(HIR_CMD_FAST_FORWARD); press(HIR_CMD_FAST_FORWARD); press(HIR_CMD_EQ);
    OFFLINE_vRun();
    press(HIR_CMD_6);
    press(HIR_CMD_POWER);
    OFFLINE_vSnake();
    /* Winner rule shared with the reference geometry. */
    {
        u8 board[9] = {1, 1, 1, 2, 2, 0, 0, 0, 0};
        assert(OFFLINE_u8Winner(board, 3, 3, 3, 1));
        assert(!OFFLINE_u8Winner(board, 3, 3, 3, 2));
    }
    {
        u8 board[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
        assert(OFFLINE_u8Winner(board, 3, 3, 3, 1));
    }
    {
        u8 board[9] = {0, 0, 1, 0, 1, 0, 1, 0, 0};
        assert(OFFLINE_u8Winner(board, 3, 3, 3, 1));
    }
    {
        u8 board[9] = {1, 2, 1, 2, 1, 2, 2, 1, 2};
        assert(!OFFLINE_u8Winner(board, 3, 3, 3, 1));
    }
    {
        u8 board[24] = {1, 1, 1, 1};
        assert(OFFLINE_u8Winner(board, 1, 4, 4, 1));
        assert(!OFFLINE_u8Winner(board, 1, 4, 5, 1));
    }
    /* Result dwell keeps Power, drops everything else. */
    press(HIR_CMD_EQ);
    assert(OFFLINE_u8Wait(1) == 0 && !HIR_u8Poll(&(HIR_Command_t){0}));
    press(HIR_CMD_POWER);
    assert(OFFLINE_u8Wait(1) == 1);
    HIR_vFlush();
    puts("PASS: boot menu, offline hub, three games, winner rules, input plumbing");
    return 0;
}
