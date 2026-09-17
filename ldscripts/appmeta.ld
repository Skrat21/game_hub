/*
 * Application metadata placement - APPLICATION image only.
 *
 * Pins the .app_metadata section (see src/APP/Common/APP_META.h) to
 * APP_META_ADDR so the bootloader can read the installed application's
 * version from a fixed address.
 *
 * The address is the last word of the application region (Sectors 2-5).
 * Intel HEX records are downloaded in ascending address order, so an
 * interrupted update leaves this word erased - which the bootloader reads as
 * "no valid application".
 */

APP_META_ADDR = DEFINED(APP_META_ADDR) ? APP_META_ADDR : 0x0803FFF0;

SECTIONS
{
    .app_metadata APP_META_ADDR : ALIGN(4)
    {
        KEEP(*(.app_metadata))
        . = ALIGN(4);
    } >FLASH
}
