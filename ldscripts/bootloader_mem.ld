/*
 * Memory Spaces Definitions - BOOTLOADER image.
 *
 * The bootloader owns Sectors 0-1 (0x08000000-0x08007FFF). It sits outside
 * the application region (Sectors 2-5) so that it survives an application
 * update performed by BOOTLOADER_u8RunOtaIfNeeded().
 *
 * The application image uses ldscripts/mem.ld instead.
 */

MEMORY
{
  RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 64K
  CCMRAM (xrw) : ORIGIN = 0x10000000, LENGTH = 0
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 32K
  FLASHB1 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB0 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB1 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB2 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB3 (rx) : ORIGIN = 0x00000000, LENGTH = 0
}