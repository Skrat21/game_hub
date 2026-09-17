#include "IR_decode.h"
#include "stm32f4xx.h"
#include "../../MCAL/EXTI/EXTI_int.h"

#define IR_LINE (1UL << 6)
#define IR_QUEUE_SIZE 16
static HIR_Decoder_t G_xDecoder;
static volatile HIR_Command_t G_xQueue[IR_QUEUE_SIZE];
static volatile u8 G_u8Head;
static volatile u8 G_u8Tail;
volatile u32 G_u32HIREdges;
volatile u32 G_u32HIRFrames;
volatile u32 G_u32HIROverflows;

u32 HIR_u32NowUS(void)
{
    return TIM2->CNT;
}

static void HIR_vEdge(void)
{
    HIR_Command_t command;
    u8 next;
    G_u32HIREdges++;
    if (!HIR_u8DecoderEdge(&G_xDecoder, HIR_u32NowUS(), &command))
        return;
    G_u32HIRFrames++;
    next = (G_u8Head + 1) % IR_QUEUE_SIZE;
    if (next == G_u8Tail) {
        G_u32HIROverflows++;
        return; /* Drop newest, never overwrite an unread command. */
    }
    G_xQueue[G_u8Head] = command;
    __DMB();
    G_u8Head = next;
}

u8 HIR_u8Poll(HIR_Command_t *command)
{
    u8 tail = G_u8Tail;
    if (command == 0 || tail == G_u8Head)
        return 0;
    __DMB();
    *command = G_xQueue[tail];
    __DMB();
    G_u8Tail = (tail + 1) % IR_QUEUE_SIZE;
    return 1;
}

void HIR_vFlush(void)
{
    u32 mask = EXTI->IMR & IR_LINE;
    EXTI->IMR &= ~IR_LINE;
    __DSB();
    G_u8Tail = G_u8Head;
    HIR_vDecoderReset(&G_xDecoder);
    EXTI->PR = IR_LINE;
    EXTI->IMR |= mask;
}

void HIR_vSuspend(void)
{
    EXTI->IMR &= ~IR_LINE;
    __DSB();
    HIR_vFlush();
}

u8 HIR_u8Init(void)
{
    u32 timer_hz, divisor, ppre;
    EXTI->IMR &= ~IR_LINE;
    /* MRCC_vInit switches away from startup PLL. Wait for that switch and
     * refresh CMSIS clock data rather than trusting stale SystemCoreClock.
     * APB1 timer clocks double when the APB1 prescaler is not unity. */
    while (((RCC->CFGR >> 2) & 3U) != (RCC->CFGR & 3U)) { }
    SystemCoreClockUpdate();
    ppre = (RCC->CFGR >> 10) & 7U;
    timer_hz = SystemCoreClock;
    if (ppre >= 4)
        timer_hz = (timer_hz >> (ppre - 3)) * 2;
    if (timer_hz < 1000000 || timer_hz % 1000000 != 0)
        return 0;
    divisor = timer_hz / 1000000;
    if (divisor > 65536)
        return 0;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    (void)RCC->APB1ENR;
    /* Receive ONLY: PA6 input/pull-up + falling EXTI6, as in the M3
     * protocol reference. TIM2 free-runs at 1 MHz for edge timestamps,
     * independently of TFT/ESP8266 SysTick delays; no timer output or AF. */
    GPIOA->MODER &= ~(3UL << 12);
    GPIOA->PUPDR = (GPIOA->PUPDR & ~(3UL << 12)) | (1UL << 12);
    SYSCFG->EXTICR[1] &= ~(15UL << 8);
    TIM2->CR1 = 0;
    TIM2->DIER = 0;
    TIM2->SMCR = 0;
    TIM2->PSC = divisor - 1;
    TIM2->ARR = 0xFFFFFFFFUL;
    TIM2->EGR = TIM_EGR_UG;
    TIM2->SR = 0;
    TIM2->CNT = 0;
    TIM2->CR1 = TIM_CR1_CEN;
    G_u8Head = G_u8Tail = 0;
    G_u32HIREdges = G_u32HIRFrames = G_u32HIROverflows = 0;
    HIR_vDecoderReset(&G_xDecoder);
    MEXTI_vSetCallBack(HIR_vEdge, EXTI_LINE6);
    EXTI->RTSR &= ~IR_LINE;
    EXTI->FTSR |= IR_LINE;
    EXTI->PR = IR_LINE;
    NVIC_SetPriority(EXTI9_5_IRQn, 1);
    NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    EXTI->IMR |= IR_LINE;
    return 1;
}
