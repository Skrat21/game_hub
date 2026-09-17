/*
 * APP_META.h
 *
 * Shared between the application image and the bootloader.
 *
 * The application image carries this metadata so the bootloader can learn
 * the version of the installed application without any cooperation from the
 * application itself. It is placed at APP_META_ADDR by the application's
 * linker script (ldscripts/appmeta.ld) and sits at the very top of the
 * application region: Intel HEX records are written in ascending address
 * order, so the metadata is the last thing an update writes. An update that
 * is interrupted therefore leaves the metadata erased, which the bootloader
 * treats as "no valid application".
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_COMMON_APP_META_H_
#define APP_COMMON_APP_META_H_

#include "../../LIB/STD_TYPES.h"

/*
 * Version of the application image. Compared against the 'version' field of
 * /status/ota.txt; the bootloader updates the device when the published
 * version is higher than this.
 */
#define APP_FW_VERSION 4u

/* Application region: STM32F401CC Sectors 2..5. */
#define APP_REGION_START 0x08008000u
#define APP_REGION_END   0x0803FFFFu

/* Fixed location of the metadata inside the application region. */
#define APP_META_ADDR  0x0803FFF0u
#define APP_META_MAGIC 0x47504D31u   /* 'GPM1' */

typedef struct
{
    u32 Magic;
    u32 Version;
    u32 Reserved0;
    u32 Reserved1;
} AppMeta_t;

#endif /* APP_COMMON_APP_META_H_ */