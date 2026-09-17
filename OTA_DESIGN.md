# OTA Implementation Design Document

## 1. Overview

This document describes the design for adding Over-The-Air (OTA) firmware update
capability to the STM32F401CC "IoT Game Monitor" project. The device currently
polls `/status/main.txt` to detect active games and renders them on the TFT
display. OTA capability is added as a parallel feature: detecting, downloading,
verifying, and flashing new firmware over the ESP8266 Wi-Fi link, with a
fallback recovery path if an update is interrupted or corrupted.

---

## 2. OTA Protocol - `/status/ota.txt`

### 2.1 Expected Format

The server serves `/status/ota.txt` in the following format:

```
version=<int>;url=<path-to-hex-file>;crc32=<hex>;size=<int>
```

Example:
```
version=2;url=/status/firmware_v2.hex;crc32=DEADBEEF;size=12345
```

**Fields:**

| Field    | Type   | Description                              |
| -------- | ------ | ---------------------------------------- |
| `version` | int   | Monotonically increasing firmware version |
| `url`     | string | HTTP path to the new firmware `.hex` file |
| `crc32`    | hex   | CRC32 checksum of the firmware data      |
| `size`     | int   | Size of the firmware in bytes            |

### 2.2 Format Notes

The format uses semicolon-separated `key=value` pairs, consistent with the
existing `main.txt` / `rps.txt` / `mathquiz.txt` status files. This allows
reusing the `CopyField()` parser already present in `main.c`.

Since `/status/ota.txt` does not yet exist in the repository, the OTA module
is written to be resilient: if the file is missing or empty, no update is
performed. The expected format when it exists is:
`version=<int>;url=<path-to-hex-file>;crc32=<hex>;size=<int>`

---

## 3. HEXPARSER Module API

### 3.1 Existing API

Location: `src/APP/HEXPARSER/`

**Header:** `HEXPARSER_int.h`
```c
void AHexParser_vParseRecord(const u8* A_s8Record);
void AHexParser_vParseData(const u8* A_s8Record);
```

**Implementation:** `HEXPARSER_prog.c`

- `AHexParser_ASCII_to_Hex(u8 A_u8ASCII)`: Converts one ASCII hex nibble to
  its numeric value. Handles '0'-'9' (0-9) and 'A'-'F' (10-15).

- `AHexParser_vParseRecord(const u8* A_s8Record)`: Reads the record type from
  byte position 8. Record type 0 = data, type 1 = EOF.

- `AHexParser_vParseData(const u8* A_s8Record)`:
  - Parses byte count from positions 1-2 (two nibbles)
  - Parses address from positions 3-6
  - Combines with `G_HIGH_ADDRESS` (0x08000000) for full address
  - Parses data bytes (2 ASCII chars per byte, starting at position 9)
  - Calls `MFMI_vProgramFlash(Address, Data[], CC/2)` to write decoded bytes

### 3.2 Checksum Handling

The existing implementation does NOT verify the Intel HEX record checksum.
This is a limitation. The enhanced streaming parser adds optional checksum
verification.

### 3.3 Enhancements for Streaming

The existing API expects a complete record string in one call. For OTA,
hex file data may arrive in fragmented chunks. We add a streaming API:

```c
void AHexParser_vInitStream(void);
u8 AHexParser_vProcessByte(u8 byte, u8 *outData, u8 *outLen);
```

- `AHexParser_vInitStream()`: Resets the internal line buffer index.
- `AHexParser_vProcessByte()`: Accumulates bytes into an internal buffer
  until a `\n` is received. Once a complete record is assembled, it parses
  it and outputs decoded binary data via `outData`/`outLen`.

### 3.4 Flash Programming Notes

`MFMI_vProgramFlash(u32 A_u32Address, u16* A_u32PrData, u32 DataLength)` writes
half-word (16-bit) data. Requires target flash to already be erased.
The OTA module erases sectors before programming.

---

## 4. Flash Layout (STM32F401CC)

### 4.1 Memory Map

The STM32F401CC has a single 256KB flash bank at `0x08000000`.

| Sector | Start Address | End Address    | Size  | Usage             |
| ------ | ------------- | -------------- | ----- | ----------------- |
| 0      | 0x08000000    | 0x08003FFF     | 16KB  | Bootloader        |
| 1      | 0x08004000    | 0x08007FFF     | 16KB  | OTA state flag    |
| 2      | 0x08008000    | 0x0800BFFF     | 16KB  | Application start |
| 3      | 0x0800C000    | 0x0800FFFF     | 16KB  | Application       |
| 4      | 0x08010000    | 0x0801FFFF     | 64KB  | Application       |
| 5      | 0x08020000    | 0x0803FFFF     | 128KB | Application       |

### 4.2 Rationale

- **Sector 0**: Smallest sector (16KB), sufficient for a minimal bootloader.
- **Sector 1**: Reserved for an OTA state flag word. The app writes
  `OTA_STATE_IN_PROGRESS` (0xA5A5A5A5) to offset 0x08004000 before flashing.
  The bootloader checks this flag on next boot.
- **Sectors 2-5**: Application code. The linker script is adjusted to place
  the app vector table at offset 0x08008000.

### 4.3 RAM Constraints

64KB total RAM. The OTA module uses:
- Streaming line buffer: 256 bytes (for one HEX record)
- Flash write buffer: up to 32 bytes (for flash programming)
- HTTP body buffer (for ota.txt): 256 bytes
- No full-image buffering.

---

## 5. Bootloader Design

### 5.1 Role

The bootloader is the first code that runs after a hardware reset. It:

1. Checks the OTA state flag at `0x08004000`.
2. If flag = `OTA_STATE_VERIFIED` or `OTA_STATE_IDLE`: Jump to app normally.
3. If flag = `OTA_STATE_IN_PROGRESS`: Clear the flag, display recovery
  message on TFT, and jump to the app.
4. If flag = `OTA_STATE_FAILED`: Clear the flag and jump to app.

### 5.2 Jump Sequence

The STM32 vector table begins with:
- Word 0: Initial Main Stack Pointer (MSP)
- Word 1: Reset vector (program counter)

The bootloader:
1. Reads MSP from `*(u32*)APP_VECTOR_START` (0x08008000)
2. Reads reset vector from `*(u32*)(APP_VECTOR_START + 4)`
3. Sets MSP via `__set_MSP()`
4. Jumps to the reset handler via function pointer

### 5.3 Files

- `src/BOOTLOADER/BOOTLOADER_int.h`
- `src/BOOTLOADER/BOOTLOADER_prog.c`

---

## 6. ESP8266 Streaming HTTP Extension

### 6.1 Problem

The existing `HESP8266_u8HttpGet()` uses `WaitForHttpBody()` which:
- Only captures text lines terminated by `\r` or `\n`
- Looks for `state=` or `status=` keywords in each line
- Returns when a matching line is found, ignoring subsequent data

This cannot handle binary `.hex` files (which contain arbitrary bytes
including `\r` and `\n` within records).

### 6.2 Solution: `HESP8266_u8HttpGetStream()`

A new function that:
1. Builds the same HTTP GET request as `HESP8266_u8HttpGet()`
2. Sends via `MUSART_vSendString()` after waiting for `>` prompt
3. Reads raw bytes from USART ring buffer using `MUSART_u8ReadReceivedByte()`
4. Skips HTTP headers by looking for `\r\n\r\n` separator
5. Passes body bytes to a user-provided callback
6. Continues until connection is closed
7. Returns `HESP8266_STATUS_OK` on success, `TIMEOUT`/`ERROR` otherwise

### 6.3 Integration

The callback receives chunks of raw bytes from the HTTP body. The OTA
module's callback feeds each byte to `AHexParser_vProcessByte()`, which
accumulates them into complete HEX records and decodes them into binary
data. Decoded data is accumulated and written to flash in half-word chunks.

---

## 7. OTA Module Design

### 7.1 State Machine

```
IDLE -> IN_PROGRESS -> VERIFIED (success, reset)
              |
              └─────> FAILED (retries exhausted, keep running)
```

| State          | Value       | Meaning                                      |
| -------------- | ----------- | -------------------------------------------- |
| OTA_IDLE       | 0xFFFFFFFF  | Default flash state (erased)                 |
| OTA_IN_PROGRESS| 0xA5A5A5A5  | Update started, not yet verified             |
| OTA_VERIFIED   | 0x5A5A5A5A  | Update complete, awaiting reset              |
| OTA_FAILED     | 0xE5E5E5E5  | Update failed after retries                  |

### 7.2 Functions

- `OTA_vCheckForUpdate(void)`: Called at boot and between games. Reads
  `/status/ota.txt`, parses version, compares with `FW_VERSION`. If newer,
  calls `OTA_u8PerformUpdate()`.

- `OTA_u8PerformUpdate(void)`: Downloads, verifies, and flashes. Returns 0 on
  success (resets before returning), non-zero on failure.

### 7.3 CRC32 Verification

Standard CRC32 with polynomial 0xEDB88320 (reversed 0x04C11DB7).

### 7.4 Retry Logic

Maximum 3 retry attempts per update cycle. On exhaustion: set state to
`OTA_FAILED`, display message on TFT, resume normal operation.

### 7.5 Progress Indication

- `HTFT_vFillRectangle()` for progress bar
- `FONT3x5_vDrawText()` for status text

### 7.6 Files

- `src/APP/OTA/OTA_int.h`
- `src/APP/OTA/OTA_prog.c`

---

## 8. Boot Sequence

1. Hardware reset -> bootloader (Sector 0) starts
2. Bootloader checks OTA flag at 0x08004000:
   - If `IN_PROGRESS`: Clear flag, display "RECOVERY" on TFT
   - Otherwise: proceed normally
3. Bootloader jumps to app at 0x08008000
4. App `main()`:
   a. InitHardware() - GPIO, SPI, USART, TFT, SysTick
   b. HESP8266_vInit()
   c. **NEW**: `OTA_vCheckForUpdate()` - check ota.txt at boot
   d. Main loop:
      - Ensure connection
      - GET `/status/main.txt`
      - Parse response for active game
      - If game active: run game loop
      - After game ends: `HESP8266_vAbort()`, game `vStop()`,
        **NEW**: `OTA_vCheckForUpdate()` - check between games
      - Loop back

---

## 9. Constraints Addressed

| Constraint | Implementation |
| ---------- | -------------- |
| No A/B flash slots | Single app region, Sectors 2-5; bootloader checks flag |
| No full image in RAM | Stream record-by-record; line buffer + flash write buffer |
| Checks only at boot and game boundaries | `OTA_vCheckForUpdate()` at boot and after each game's vStop() |
| Capped retries (3) | Implemented in `OTA_u8PerformUpdate()` |
| Graceful fallback | App-level failures keep running firmware |
| Reuse existing primitives | HEXPARSER, FONT3x5, HTFT_vFillRectangle, USART ring buffer |
| Website out of scope | Only reading `/status/ota.txt`; no website files modified |