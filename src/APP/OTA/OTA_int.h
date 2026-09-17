/*
 * OTA_int.h
 *
 * Application-side OTA trigger detection.
 *
 * The application deliberately does NOT download or flash firmware: it runs
 * from the application region (Sectors 2-5), so erasing that region from
 * inside the application would destroy the code performing the erase. The
 * bootloader lives in Sectors 0-1, outside the region it writes, and owns
 * the OTA engine.
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#ifndef APP_OTA_OTA_INT_H_
#define APP_OTA_OTA_INT_H_

#include "../../LIB/STD_TYPES.h"

#define OTA_STATUS_PATH "/status/ota.txt"

/*
 * Enable only once the bootloader's OTA engine is installed: the
 * application then reboots so the bootloader can download and flash the
 * published image. Left at 0, a detected update is reported on the TFT but
 * the device keeps running normally.
 */
#define APP_OTA_REBOOT_ENABLED 0

/*
 * Reads /status/ota.txt and reports when a newer firmware is published:
 * connects (no socket is live when this is called), reads the trigger file
 * and compares its 'version' field against APP_FW_VERSION.
 *
 * Called at boot and between games.
 */
void OTA_vCheckForUpdate(void);

#endif /* APP_OTA_OTA_INT_H_ */