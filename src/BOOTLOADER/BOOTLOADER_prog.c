/*
 * BOOTLOADER_prog.c
 *
 * Bootloader for the STM32F401CC IoT Game Monitor.
 *
 * Runs first on every reset, from Sectors 0-1 (0x08000000-0x08007FFF). It
 * sits outside the application region (Sectors 2-5, 0x08008000-0x0803FFFF),
 * so it is not affected while the application region is erased and
 * reprogrammed.
 *
 * Responsibilities:
 *   1. decide whether a valid application is installed;
 *   2. (staged) download and flash a newer application published by the
 *      website, then jump to it;
 *   3. jump to the installed application.
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#include <stm32f4xx.h>

#include "../LIB/STD_TYPES.h"

#include "../APP/Common/APP_META.h"
#include "BOOTLOADER_int.h"

/* ------------------------------------------------------------------
 * Application image inspection
 * ------------------------------------------------------------------ */

static u32 BOOTLOADER_u32ReadWord(u32 address)
{
    return *(volatile u32 *) address;
}

u8 BOOTLOADER_u8AppIsValid(void)
{
    const AppMeta_t *meta = (const AppMeta_t *) APP_META_ADDR;
    u32 stack_pointer = BOOTLOADER_u32ReadWord(APP_REGION_START);
    u32 reset_vector = BOOTLOADER_u32ReadWord(APP_REGION_START + 4u);

    /* The metadata is written last by an update, so a missing magic means
     * either "never programmed" or "update interrupted". */
    if (meta->Magic != APP_META_MAGIC)
    {
        return 0;
    }

    /* Initial stack pointer must point into the 64 KB of internal SRAM. */
    if (stack_pointer < 0x20000000u || stack_pointer > 0x20010000u)
    {
        return 0;
    }

    /* Reset vector must be a Thumb address inside the application region. */
    if (reset_vector < (APP_REGION_START + 8u) || reset_vector > APP_META_ADDR)
    {
        return 0;
    }
    if ((reset_vector & 1u) == 0u)
    {
        return 0;
    }

    return 1;
}

u32 BOOTLOADER_u32GetAppVersion(void)
{
    const AppMeta_t *meta = (const AppMeta_t *) APP_META_ADDR;

    if (!BOOTLOADER_u8AppIsValid())
    {
        return 0u;
    }
    return meta->Version;
}

/* ------------------------------------------------------------------
 * Control transfer
 * ------------------------------------------------------------------ */

void VBOOTLOADER_vJumpToApp(void)
{
    u32 app_stack_pointer = BOOTLOADER_u32ReadWord(APP_REGION_START);
    u32 app_reset_vector = BOOTLOADER_u32ReadWord(APP_REGION_START + 4u);

    __asm volatile ("cpsid i" ::: "memory");

    /* Stop the bootloader tick before handing over. */
    SysTick->CTRL = 0u;
    SysTick->LOAD = 0u;
    SysTick->VAL = 0u;

    /* Point the core at the application's vector table. */
    SCB->VTOR = APP_REGION_START;
    __DSB();
    __ISB();

    /* Load the application stack pointer and branch to its reset vector.
     * The application's own startup code finishes the bring-up. */
    __asm volatile
    (
        "msr msp, %0 \n"
        "bx  %1      \n"
        :
        : "r" (app_stack_pointer), "r" (app_reset_vector)
        : "memory"
    );

    /* Not reached. */
    for (;;)
    {
    }
}

/* ------------------------------------------------------------------
 * OTA
 * ------------------------------------------------------------------ */

/*
 * Reports whether an update is published and flashes it.
 *
 * Not implemented yet: this currently reports "nothing to do" so the device
 * behaviour is unchanged. The application-side trigger detection, the shared
 * text HTTP reader and the flash layout are already in place for it.
 */
u8 BOOTLOADER_u8RunOtaIfNeeded(void)
{
    return 0u;
}

/* ------------------------------------------------------------------
 * Entry point
 * ------------------------------------------------------------------ */

int main(void)
{
    /* (staged) BOOTLOADER_u8RunOtaIfNeeded() will bring the Wi-Fi link up
     * and flash a newer image here, before any application is started. */

    if (!BOOTLOADER_u8AppIsValid())
    {
        /* No usable application in the application region. Stay in the
         * bootloader so it can be seen/recovered instead of branching into
         * empty flash. */
        for (;;)
        {
        }
    }

    VBOOTLOADER_vJumpToApp();

    return 0;
}