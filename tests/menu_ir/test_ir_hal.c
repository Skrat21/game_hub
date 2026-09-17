#include <assert.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "../../src/HAL/IR/IR_int.h"
TestTimer test_timer;
TestRcc test_rcc;
TestGpio test_gpio;
TestSyscfg test_syscfg;
TestExti test_exti;
uint32_t SystemCoreClock = 25000000;
static void (*callback)(void);
void SystemCoreClockUpdate(void) { }
void NVIC_SetPriority(int irq, unsigned priority) { assert(irq == 23 && priority == 1); }
void NVIC_ClearPendingIRQ(int irq) { assert(irq == 23); }
void NVIC_EnableIRQ(int irq) { assert(irq == 23); }
void MEXTI_vSetCallBack(void (*fn)(void), u8 line) { assert(line == 6); callback = fn; }
static void edge(u32 delta) { test_timer.CNT += delta; callback(); }
static void frame(u8 code)
{
    u32 bits = 0xFF00UL | ((u32)code << 16) | ((u32)(code ^ 255) << 24);
    unsigned bit;
    edge(50000); edge(13500);
    for (bit = 0; bit < 32; bit++) edge(bits & ((u32)1 << bit) ? 2250 : 1125);
}
int main(void)
{
    unsigned i;
    HIR_Command_t cmd;
    test_rcc.CFGR = 5; /* HSE selected and active, AHB/APB1 unity. */
    test_gpio.MODER = 0xFFFFFFFF;
    assert(HIR_u8Init());
    assert(test_timer.PSC == 24 && test_timer.ARR == 0xFFFFFFFFUL);
    assert((test_gpio.MODER & (3U << 12)) == 0);
    assert((test_gpio.PUPDR & (3U << 12)) == (1U << 12));
    assert((test_exti.FTSR & 64) && !(test_exti.RTSR & 64));
    assert(test_exti.IMR & 64);
    assert(!HIR_u8Poll(&cmd) && !HIR_u8Poll(0));
    frame(HIR_CMD_EQ);
    assert(G_u32HIREdges == 34 && G_u32HIRFrames == 1);
    assert(HIR_u8Poll(&cmd) && cmd.Address == 0 && cmd.Command == HIR_CMD_EQ);
    assert(!HIR_u8Poll(&cmd));
    for (i = 0; i < 20; i++) frame((u8)i);
    assert(G_u32HIROverflows == 5);
    for (i = 0; i < 15; i++) assert(HIR_u8Poll(&cmd) && cmd.Command == i);
    assert(!HIR_u8Poll(&cmd));
    frame(7); HIR_vFlush(); assert(!HIR_u8Poll(&cmd));
    HIR_vSuspend(); assert(!(test_exti.IMR & 64));
    test_rcc.CFGR = 5 | (4U << 10); /* APB1 /2, timer still 25 MHz */
    assert(HIR_u8Init() && test_timer.PSC == 24);
    test_rcc.CFGR = 5 | (5U << 10); /* APB1 /4: 12.5 MHz cannot divide to 1 MHz */
    assert(!HIR_u8Init() && !(test_exti.IMR & 64));
    puts("PASS: actual IR HAL init, callback -> decoder -> FIFO, overflow, flush, suspend, APB clocks");
    return 0;
}
