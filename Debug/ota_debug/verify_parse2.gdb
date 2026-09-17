# Verifies the version gate of OTA_vCheckForUpdate() with a synthetic ota.txt,
# driven by explicit top-level continues (no auto-continue in command lists).
#
# Disassembly of the compare (build under test):
#   800fbf2: ldr  r3,[r7,#364]        ; server_version
#   800fbf6: cmp  r3,#4
#   800fbf8: bhi  800fc02             ; -> UPDATE FOUND when server_version > 4

set pagination off
set confirm off
set remotetimeout 120
set print asm-demangle on

file "Debug/fixed project.elf"
target extended-remote localhost:3333
monitor reset halt
delete

# --- boot cleanly, then divert into RunOnlineMode() for a live frame -------
break MENU_enChooseMode
commands
silent
continue
end

break ../src/APP/OTA/OTA_prog.c:162
commands
silent
printf "\n@@@ live frame: OTA_vCheckForUpdate() at its network call, body=%p (256 bytes)\n", &body
continue
end

printf "\n=== resetting and booting the application ===\n"
monitor reset run
# the MENU_enChooseMode and line-162 breakpoints will stop the target
continue

# --- the frame is now live; install outcome-only breakpoints ---------------
# remove the helpers so they do not fire during the scenarios
delete 1 2

break ../src/APP/OTA/OTA_prog.c:198   ; commands; silent; printf "\n@@@ PARSED: version='%s' url='%s' crc32='%s' size='%s'  server_version=%u\n", version_str, url_str, crc_str, size_str, server_version; end
break ../src/APP/OTA/OTA_prog.c:209   ; commands; silent; printf "\n@@@ >>> UPDATE FOUND: server %u > APP_FW_VERSION 4\n", server_version; end
break ../src/APP/OTA/OTA_prog.c:202   ; commands; silent; printf "\n@@@ >>> NO UPDATE: server %u <= APP_FW_VERSION 4\n", server_version; end
break ../src/APP/OTA/OTA_prog.c:193   ; commands; silent; printf "\n@@@ >>> MISSING/invalid fields\n"; end

printf "\n=== SCENARIO A: version=9  -> expect UPDATE FOUND ===\n"
set var body = "version=9;url=http://185.176.43.100/fw/app.hex;crc32=ABCD1234;size=9999;"
set $pc = (void*) 0x0800fb86
continue        # parse block -> stops at 198 (parsed print)
printf "    after line 198: PC = %p  server_version = %u\n", $pc, server_version
continue        # the compare -> should stop at 209 (UPDATE FOUND)
printf "    after the compare: PC = %p\n", $pc

printf "\n=== SCENARIO B: version=3  -> expect NO UPDATE ===\n"
monitor halt
set var body = "version=3;url=http://185.176.43.100/fw/app.hex;crc32=ABCD1234;size=9999;"
set $pc = (void*) 0x0800fb86
continue
printf "    after line 198: PC = %p  server_version = %u\n", $pc, server_version
continue
printf "    after the compare: PC = %p\n", $pc

printf "\n=== SCENARIO C: malformed ota.txt  -> expect MISSING FIELDS ===\n"
monitor halt
set var body = "garbage"
set $pc = (void*) 0x0800fb86
continue
printf "    after parse: PC = %p\n", $pc

printf "\n@@@ PARSE/GATE VERIFICATION COMPLETE\n"
delete
monitor reset run
detach
quit