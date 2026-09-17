/*
 * BOOTLOADER_int.h
 *
 * Bootloader interface. The bootloader occupies Sectors 0-1
 * (0x08000000-0x08007FFF) of the STM32F401CC, outside the application
 * region (Sectors 2-5), so it survives an application update.
 *
 *  Created on: Sep 16, 2026
 *      Author: AHMED SHERIF
 */

#ifndef BOOTLOADER_BOOTLOADER_INT_H_
#define BOOTLOADER_BOOTLOADER_INT_H_

#include "../LIB/STD_TYPES.h"

/*
 * Reads APP_META_ADDR and the application's vector table and reports
 * whether a usable application image is present at APP_REGION_START.
 */
u8 BOOTLOADER_u8AppIsValid(void);

/* Version of the installed application, or 0 when none is present. */
u32 BOOTLOADER_u32GetAppVersion(void);

/* Hands control to the application: loads its stack pointer and branches to
 * its reset vector. Never returns. */
void VBOOTLOADER_vJumpToApp(void);

/*
 * Checks /status/ota.txt and downloads + flashes a newer application image
 * into Sectors 2-5. Returns 1 when the application region was modified,
 * 0 when there was nothing to do. Never returns on a failure that leaves
 * no usable application.
 */
u8 BOOTLOADER_u8RunOtaIfNeeded(void);

#endif /* BOOTLOADER_BOOTLOADER_INT_H_ */