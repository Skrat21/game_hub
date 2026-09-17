# Verifies the OTA trigger parsing and the version gate in
# OTA_vCheckForUpdate() on real hardware, without needing the Wi-Fi AP.
#
# The network leg (EnsureConnection + HttpGetText) is jumped over and the
# local `body` buffer is filled with a synthetic /status/ota.txt, so the REAL
# compiled parser (OTA_u8CopyField / OTA_u32ParseNumber) and the REAL
# comparison against APP_FW_VERSION are the code being exercised.
#
# Line-to-address map for the build under test (0x0800fb86 = line 186):
#   162  EnsureConnection call      - a frame owning `body` exists here
#   186  start of the field-parse block
#   193  malformed / missing fields -> return
#   198  version comparison
#   202  "no update, keep running" return
#   209  "UPDATE FOUND"

set pagination off
set confirm off
set remotetimeout 120

file "Debug/fixed project.elf"
target extended-remote localhost:3333

# Start from a real reset: this also clears any stale hardware breakpoints
# left in the FPB by previous sessions.
monitor reset halt
delete

# --- boot normally, then divert into RunOnlineMode() -----------------------
break MENU_enChooseMode
commands
silent
printf "\n@@@ the application booted normally (menu reached at %p)\n", $pc
printf "@@@ diverting into RunOnlineMode() so OTA_vCheckForUpdate() runs\n"
set $pc = (void*) 0x08013fb0
continue
end

break ../src/APP/OTA/OTA_prog.c:162
commands
silent
printf "\n@@@ live frame: OTA_vCheckForUpdate() paused at its network call\n"
printf "    body[] is at %p (%u bytes)\n", &body, sizeof(body)
end

continue

delete

# --- breakpoints on the parser's outcomes ----------------------------------
break ../src/APP/OTA/OTA_prog.c:198
commands
silent
printf "\n@@@ PARSED ota.txt -> version='%s' url='%s' crc32='%s' size='%s'\n", version_str, url_str, crc_str, size_str
printf "    server_version = %u\n", server_version
continue
end

break ../src/APP/OTA/OTA_prog.c:193
commands
silent
printf "\n@@@ branch: FIELDS MISSING -> no update. body was <<<%s>>>\n", body
end

break ../src/APP/OTA/OTA_prog.c:202
commands
silent
printf "\n@@@ branch: server %u <= APP_FW_VERSION 4 -> NO UPDATE, keep running\n", server_version
end

break ../src/APP/OTA/OTA_prog.c:209
commands
silent
printf "\n@@@ branch: server %u > APP_FW_VERSION 4 -> *** UPDATE FOUND ***\n", server_version
end

# --- SCENARIO A: a newer firmware is published -----------------------------
printf "\n=== SCENARIO A: website publishes version=9 (newer than 4) ===\n"
set var body = "version=9;url=http://185.176.43.100/fw/app.hex;crc32=ABCD1234;size=9999;"
set $pc = (void*) 0x0800fb86
continue

# --- SCENARIO B: the server is not ahead ----------------------------------
printf "\n=== SCENARIO B: website publishes version=3 (older than 4) ===\n"
set var body = "version=3;url=http://185.176.43.100/fw/app.hex;crc32=ABCD1234;size=9999;"
set $pc = (void*) 0x0800fb86
continue

# --- SCENARIO C: ota.txt is not in the expected format --------------------
printf "\n=== SCENARIO C: malformed ota.txt ===\n"
set var body = "garbage"
set $pc = (void*) 0x0800fb86
continue

printf "\n@@@ PARSE/GATE VERIFICATION COMPLETE\n"

delete
monitor reset run
detach
quit