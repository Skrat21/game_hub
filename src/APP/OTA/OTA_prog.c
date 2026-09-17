/*
 * OTA_prog.c
 *
 * Application-side OTA trigger detection for the STM32F401CC IoT Game
 * Monitor.
 *
 * The device is triggered from the website: publishing firmware writes
 * /status/ota.txt, and this module polls it. Downloading and flashing are
 * performed by the bootloader (src/BOOTLOADER), which lives outside the
 * application region; from here we only detect and report.
 *
 *  Created on: Sep 17, 2026
 *      Author: AHMED SHERIF
 */

#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPES.h"

#include "../../MCAL/SYSTICK/SYSTICK_int.h"

#include "../../HAL/ESP8266/ESP8266_int.h"
#include "../../HAL/ESP8266/ESP8266_cfg.h"
#include "../../HAL/TFT/TFT_int.h"
#include "../../HAL/OTA_LED/OTA_LED_int.h"

#include "../Common/APP_META.h"
#include "OTA_int.h"

#define OTA_STATUS_BODY_SIZE 256
#define CONNECTION_RETRIES   3

/*
 * Image metadata read by the bootloader (see APP_META.h). Placed at
 * APP_META_ADDR by ldscripts/appmeta.ld.
 */
const AppMeta_t G_xAppMeta
    __attribute__((section(".app_metadata"), used)) =
{
    APP_META_MAGIC,
    APP_FW_VERSION,
    0u,
    0u
};

/* ---------------------------------------------------------------
 * Parsing helpers (same key=value; protocol the game status files use)
 * --------------------------------------------------------------- */

static u8 OTA_u8CopyField(const char *body, const char *field,
        char *value, u16 value_size)
{
    const char *cursor = body;
    u16 field_length = 0;

    while (field[field_length] != '\0') field_length++;

    while (*cursor != '\0')
    {
        const char *field_start = cursor;
        const char *value_start;
        u16 value_length = 0;
        u16 index;

        while (*cursor != '\0' && *cursor != '=' && *cursor != ';')
        {
            cursor++;
        }

        if (*cursor == '=' && (u16)(cursor - field_start) == field_length)
        {
            u8 matches = 1;
            for (index = 0; index < field_length; index++)
            {
                if (field_start[index] != field[index]) matches = 0;
            }

            value_start = ++cursor;
            while (*cursor != '\0' && *cursor != ';') cursor++;

            while (value_start + value_length < cursor) value_length++;

            if (matches && value_size > 0)
            {
                if (value_length >= value_size) value_length = value_size - 1;
                for (index = 0; index < value_length; index++)
                    value[index] = value_start[index];
                value[value_length] = '\0';
                return 1;
            }
        }
        else
        {
            while (*cursor != '\0' && *cursor != ';') cursor++;
        }

        if (*cursor == ';') cursor++;
    }

    return 0;
}

static u32 OTA_u32ParseNumber(const char *text)
{
    u32 value = 0;

    while (*text >= '0' && *text <= '9')
    {
        value = value * 10 + (u32)(*text - '0');
        text++;
    }
    return value;
}

/* ---------------------------------------------------------------
 * Display / reset helpers
 * --------------------------------------------------------------- */

static void OTA_vShowStatus(const char *text, u16 color)
{
    HTFT_vFillBackgroundColor(TFT_BLACK);
    FONT3x5_vDrawText(text, 40, 70, color);
}

#if APP_OTA_REBOOT_ENABLED
static void OTA_vSystemReset(void)
{
    __asm volatile(
        "ldr r0, =0xE000ED0C\n"
        "ldr r1, =0x05FA0004\n"
        "str r1, [r0]\n"
        "dsb\n"
        "isb\n"
        :
        :
        : "r0", "r1", "memory"
    );

    while (1)
    {
    }
}
#endif

/* ---------------------------------------------------------------
 * Public API
 * --------------------------------------------------------------- */

void OTA_vCheckForUpdate(void)
{
    char body[OTA_STATUS_BODY_SIZE];
    char version_str[12];
    char url_str[64];
    char crc_str[16];
    char size_str[12];
    u32 server_version;

    /*
     * This runs right after HESP8266_vInit() at boot and right after
     * HESP8266_vAbort() between games, so no TCP connection is live yet and
     * HESP8266_u8HttpGet*() refuse to send without one.
     */
    if (HESP8266_u8EnsureConnection(HESP8266_WIFI_SSID,
            HESP8266_WIFI_PASSWORD, HESP8266_SERVER_IP,
            HESP8266_SERVER_PORT, CONNECTION_RETRIES)
            != HESP8266_STATUS_OK)
    {
        HESP8266_vAbort();
        OTA_LED_vSetActive(0);
        return;
    }

    /*
     * /status/ota.txt is a plain key=value; line, so it needs the text
     * reader: HESP8266_u8HttpGet() only ever returns a line containing
     * "state=" or "status=", which the OTA line never has.
     */
    if (HESP8266_u8HttpGetText(OTA_STATUS_PATH, body, sizeof(body))
            != HESP8266_STATUS_OK)
    {
        HESP8266_vAbort();
        OTA_LED_vSetActive(0);
        return;
    }
    HESP8266_vCloseConnection();

    if (!OTA_u8CopyField(body, "version", version_str, sizeof(version_str))
            || !OTA_u8CopyField(body, "url", url_str, sizeof(url_str))
            || !OTA_u8CopyField(body, "crc32", crc_str, sizeof(crc_str))
            || !OTA_u8CopyField(body, "size", size_str, sizeof(size_str)))
    {
        /* Malformed or empty ota.txt - no update */
        OTA_LED_vSetActive(0);
        return;
    }

    server_version = OTA_u32ParseNumber(version_str);

    if (server_version <= APP_FW_VERSION)
    {
        /* Already running current firmware */
        OTA_LED_vSetActive(0);
        return;
    }

    /*
     * An update is available. The download and the flashing are performed by
     * the bootloader, which lives outside the application region.
     */
    OTA_vShowStatus("UPDATE FOUND", TFT_CYAN);
    OTA_LED_vSetActive(1);
    MSYSTICK_vSetDelay_ms(2000);
#if APP_OTA_REBOOT_ENABLED
    OTA_vShowStatus("REBOOTING", TFT_GREEN);
    MSYSTICK_vSetDelay_ms(1000);
    OTA_LED_vSetActive(0);
    OTA_vSystemReset();
#else
    /* Bootloader OTA engine not installed yet: report and carry on. */
    OTA_LED_vSetActive(0);
    HTFT_vFillBackgroundColor(TFT_BLACK);
#endif
}
