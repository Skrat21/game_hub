/* Host register model, selected only by the host test include path. */
#ifndef TEST_STM32F4XX_H_
#define TEST_STM32F4XX_H_
#include <stdint.h>
typedef struct { uint32_t CR1, DIER, SMCR, PSC, ARR, EGR, SR, CNT; } TestTimer;
typedef struct { uint32_t CFGR, AHB1ENR, APB1ENR, APB2ENR; } TestRcc;
typedef struct { uint32_t MODER, PUPDR; } TestGpio;
typedef struct { uint32_t EXTICR[4]; } TestSyscfg;
typedef struct { uint32_t IMR, PR, RTSR, FTSR; } TestExti;
extern TestTimer test_timer;
extern TestRcc test_rcc;
extern TestGpio test_gpio;
extern TestSyscfg test_syscfg;
extern TestExti test_exti;
extern uint32_t SystemCoreClock;
#define TIM2 (&test_timer)
#define RCC (&test_rcc)
#define GPIOA (&test_gpio)
#define SYSCFG (&test_syscfg)
#define EXTI (&test_exti)
#define RCC_AHB1ENR_GPIOAEN 1U
#define RCC_APB2ENR_SYSCFGEN (1U << 14)
#define RCC_APB1ENR_TIM2EN 1U
#define TIM_EGR_UG 1U
#define TIM_CR1_CEN 1U
#define EXTI9_5_IRQn 23
#define __DMB() ((void)0)
#define __DSB() ((void)0)
void SystemCoreClockUpdate(void);
void NVIC_SetPriority(int irq, unsigned priority);
void NVIC_ClearPendingIRQ(int irq);
void NVIC_EnableIRQ(int irq);
#endif
