# Task: Implement Over-The-Air (OTA) Firmware Updates (Firmware Only)

You are extending the STM32F401CC "IoT Game Monitor" project with OTA firmware
update capability. The device currently polls `/status/main.txt` to detect
active games (RPS, Math Quiz, Snake and Ladder) and renders them on the TFT.
You are adding a parallel capability: detecting, downloading, verifying, and
flashing new firmware over the same ESP8266 Wi-Fi link, with a fallback
recovery path if an update is interrupted or corrupted.

**Scope boundary — read carefully:** The website (`website/Projecth/**`) is
OUT OF SCOPE for changes. You must NOT create, edit, or delete any file under
`website/Projecth/`. Assume `/status/ota.txt` already exists and is served by
the backend in the form:
```
version=<int>;url=<path-to-hex-file>;crc32=<hex>;size=<int>
```
Read it, do not build it. If its actual format differs from this once you
inspect it, adapt to the real format and document the difference — do not
request website changes.

---

## PHASE 0 — Analysis (do this before writing any code)

1. **Inspect `/status/ota.txt`** (read-only) to confirm the real field names
   and value formats. Document them in `OTA_DESIGN.md` at the repo root.
2. **Inspect `src/APP/HEXPARSER/`** thoroughly — this Intel HEX record parser
   already exists in the project and MUST be reused (not duplicated) for
   decoding the downloaded firmware image record-by-record. Document its
   existing API (function signatures, record struct shape, checksum handling)
   in `OTA_DESIGN.md` so the OTA module can call into it correctly.
3. **Inspect the linker script(s)** in `ldscripts/` to understand current
   flash sector layout and how much headroom exists. Document sector
   boundaries and sizes in `OTA_DESIGN.md`.
4. **Inspect `HESP8266_u8HttpGet()`** in `src/HAL/ESP8266/ESP8266_prg.c` — it
   is currently written for text-based, line-oriented responses (`state=` /
   `status=` matching). Determine what changes are needed to support a
   binary-safe, chunked/streaming GET of a `.hex` file without assuming
   newline-delimited text, and document the plan (new function, e.g.
   `HESP8266_u8HttpGetStream()`, rather than repurposing the existing one and
   risking breaking RPS/Math Quiz/Snake and Ladder polling).
5. **Constraints to design around** (confirmed, not open questions):
   - Flash is single-bank, 256KB, no A/B slot swap. Do not build two full app
     copies — the binary is expected to grow.
   - RAM is 64KB total. Do NOT buffer the entire firmware image in RAM before
     flashing. Stream and flash incrementally, one flash sector's worth of
     decoded bytes at a time.
   - OTA checks happen (a) once at boot, and (b) between games — specifically
     when a game's `state=DEAD` / `status=ended` transition returns the main
     loop to `PollMainStatus`, before the next game session begins. Do not
     check mid-game.
   - Version comparison is against a compile-time constant (e.g.
     `#define FW_VERSION <int>` in a header), compared against `/status/ota.txt`'s
     `version=` field. If `version` on the server is greater, an update
     triggers.
   - Retries: allow a reasonable, capped number of retry attempts (e.g. 3) per
     update attempt cycle for both the initial download and CRC verification.
     If retries are exhausted, abort the update, keep running the current
     firmware, show a failure message on the TFT, and resume normal operation
     (do not enter a hard-fail state or brick the device from a failed OTA
     attempt on a device that was already running fine).

Do not proceed past this phase until `OTA_DESIGN.md` reflects real findings
from the four inspection steps above, not assumptions.

---

## PHASE 1 — Recovery Bootloader

- Create a new build target / source area (e.g. `src/BOOTLOADER/`) for a
  minimal bootloader that occupies a small, fixed, protected flash sector
  (the first 16KB sector, per the F401CC's uneven sector layout — confirm
  exact size against Phase 0's linker script findings).
- The bootloader is the actual reset entry point. On every boot it:
  1. Reads a persisted **OTA state record** from a dedicated flash page
     (state: `IDLE`, `IN_PROGRESS`, `VERIFIED`, `FAILED`).
  2. If state is `IDLE` or `VERIFIED`, jump immediately to the application
     (minimize boot latency — this is the normal-boot path).
  3. If state is `IN_PROGRESS` or `FAILED` (meaning a prior update did not
     complete cleanly and the app region may be inconsistent), do NOT jump to
     the app. Instead, bring up just enough of the stack (clocks, USART1,
     ESP8266 driver, minimal TFT message) to re-attempt the OTA fetch against
     `/status/ota.txt` directly from the bootloader, up to the capped retry
     count from Phase 0. If it succeeds, mark `VERIFIED` and jump to the app.
     If retries are exhausted here too, display a clear TFT message (e.g.
     `"UPDATE FAILED - REFLASH REQUIRED"`) and halt — this is the one
     legitimate case where manual SWD reflashing is the expected fallback,
     since the app region is genuinely not trustworthy.
- The bootloader must keep its own minimal copy of whatever config it needs
  (Wi-Fi credentials, server IP) — reuse `ESP8266_cfg.h` values at build time
  rather than inventing a second config mechanism.
- Update the linker script(s) to place the bootloader in its own protected
  region, the application in the remaining flash, and reserve one small flash
  page (separate from both) for the persisted OTA state record.

---

## PHASE 2 — OTA State & Version Management

- Create `src/APP/OTA/OTA_int.h` and `OTA_prg.c`.
- Define `FW_VERSION` as a compile-time constant in a shared header, included
  by both the bootloader and application, incremented manually per release.
- Implement read/write helpers for the persisted OTA state record (state enum
  + last-known-good version + retry counter), stored in the reserved flash
  page from Phase 1. These helpers are shared by both bootloader and app code
  (put them somewhere both build targets can include, not duplicated).
- Implement `OTA_u8CheckForUpdate(void)`: polls `/status/ota.txt` via the
  existing HTTP GET path (this one's small and text-based, so the existing
  `HESP8266_u8HttpGet()` is fine here), parses `version`, `url`, `crc32`,
  `size`, and returns whether `version > FW_VERSION`.

---

## PHASE 3 — Streaming Download, Parse & Flash

- Implement `HESP8266_u8HttpGetStream()` (or equivalent) in the ESP8266 HAL
  per the Phase 0 findings — binary-safe, chunked reads from the ring buffer,
  no assumption of line-delimited text.
- Implement `OTA_u8PerformUpdate(url, expected_crc32, expected_size)`:
  1. Set persisted state to `IN_PROGRESS` before writing any flash (this is
     the flag the bootloader checks on next boot if power is lost mid-update).
  2. Stream the `.hex` file in chunks. Feed each chunk into the existing
     `HEXPARSER` module record-by-record as it arrives — do not wait for the
     full file before parsing.
  3. Accumulate each record's decoded payload bytes into a small RAM staging
     buffer sized to one flash sector (not the whole image). When the staging
     buffer fills a sector's worth of data (or the image ends), erase that
     target flash sector and write the staged bytes, then clear the staging
     buffer and continue.
  4. Maintain a running CRC32 over all decoded bytes as they're produced by
     the parser (not just per-sector) so the full-image CRC can be checked at
     the end.
  5. Rely on Intel HEX's own per-record checksum (via `HEXPARSER`) to catch
     transmission corruption at the record level, in addition to the overall
     CRC32 check.
  6. If a chunk read, HEX record checksum, or flash write fails, retry per the
     capped retry count from Phase 0. If retries are exhausted, set state to
     `FAILED`, do NOT jump anywhere new (the app currently running keeps
     running since it hasn't been touched — only the *inactive*, not-yet-fully-
     written portion of flash is questionable), and return failure to the
     caller so it can show a TFT message and resume normal operation.
  7. On success, compare the final running CRC32 against `expected_crc32`. If
     it matches, set state to `VERIFIED`, update the persisted last-known-good
     version, and trigger a software reset so the bootloader boots the new
     image. If it doesn't match, treat as failure per step 6.

**Important:** because there is no second app slot, an update in progress is
overwriting the region the running app will need on next boot. This means the
device MUST NOT be allowed to reset or lose power confidence between "state
set to IN_PROGRESS" and "state set to VERIFIED" — this is exactly why the
bootloader's retry-on-`IN_PROGRESS`/`FAILED` path in Phase 1 exists as the
safety net. Make sure Phase 1 and Phase 3 agree on the exact state values and
flash page location.

---

## PHASE 4 — Application Integration & TFT Feedback

- At boot (in `main.c`, after normal hardware init but before entering
  `PollMainStatus`), call `OTA_u8CheckForUpdate()`. If an update is available,
  display `"NEW UPDATE DETECTED!"` via the existing 3x5 font renderer, then
  call `OTA_u8PerformUpdate()`. While it runs, show a simple progress
  indicator (e.g. a filling bar via `HTFT_vFillRectangle`, updated per sector
  written or per N chunks received) — reuse existing drawing primitives, do
  not build new ones unnecessarily.
- In the `PollMainStatus` loop, add the same check at the point where a game
  ends and control returns to `PollMainStatus` (i.e. after
  `RPS_vStop()` / `MathQuiz_vStop()` / `SnakeLadder_vStop()`, before the next
  `SendGetMain` poll) — not mid-game.
- On update failure (retries exhausted), show a brief, clear TFT message
  (e.g. `"UPDATE FAILED - RUNNING v<FW_VERSION>"`) for a short hold time (match
  existing UI hold conventions, e.g. ~2000ms), then resume normal operation
  exactly as if no update had been detected.
- On update success, the device resets itself — no special handling needed in
  `main.c` beyond the reset call, since the bootloader takes over from there.

---

## PHASE 5 — Testing & Docs

- Test the full happy path: bump `FW_VERSION`, serve a valid `.hex` +
  correct `crc32`/`size` via a mocked or real `/status/ota.txt`, confirm the
  device detects it at boot and between games, downloads, flashes, resets,
  and boots the new version.
- Test corruption handling: serve a deliberately wrong `crc32` and confirm the
  device retries the capped number of times, then fails gracefully and keeps
  running the old firmware with a TFT message — and does NOT get stuck in a
  reboot loop.
- Test interrupted-update recovery: simulate a power loss (or forced reset)
  while state is `IN_PROGRESS`, and confirm the bootloader detects this on
  next boot and re-attempts the OTA fetch rather than jumping into a
  potentially half-written app region.
- Update the root `README.md`: add an "Over-The-Air Updates" section
  describing the bootloader/app split, the `/status/ota.txt` protocol fields,
  the state machine (`IDLE`/`IN_PROGRESS`/`VERIFIED`/`FAILED`), and update the
  file structure tree to include `src/BOOTLOADER/` and `src/APP/OTA/`. Do NOT
  touch website-related sections beyond noting the new protocol file if
  needed for accuracy.

---

## Constraints (non-negotiable)

- **Never create, edit, or delete anything under `website/Projecth/`.** Read
  `/status/ota.txt` only.
- Never buffer the full firmware image in RAM — stream and flash sector by
  sector, using `HEXPARSER` for record-level decoding as data arrives.
- No A/B flash slots — single app region, protected by the bootloader's
  retry-on-incomplete-update logic instead.
- OTA checks occur only at boot and at game-end transitions back to
  `PollMainStatus` — never mid-game.
- Capped, reasonable retry count (not infinite) for both download/flash
  failures and CRC mismatches, with graceful fallback to the currently
  running firmware and a TFT status message on exhaustion — except in the
  bootloader's own recovery path, where exhausting retries on an already-
  incomplete update is a legitimate hard-fail state requiring manual reflash.
- Reuse existing primitives everywhere possible: `HEXPARSER` for record
  parsing, the 3x5 font renderer for all TFT text, `HTFT_vFillRectangle` /
  existing drawing calls for the progress indicator, and the existing ESP8266
  driver/ring buffer infrastructure (extended, not forked, for streaming
  reads).
- Keep RPS, Math Quiz, and Snake and Ladder fully functional — this is an
  additive capability that wraps around the existing main loop, not a
  rewrite of it.
