set pagination off
set confirm off
set remotetimeout 300

file "Debug/fixed project.elf"
target extended-remote localhost:3333
monitor halt
delete

# OTA_vCheckForUpdate() runs as the very first thing in RunOnlineMode()
# (main.c:715), so entering RunOnlineMode() exercises the real path.

break ../src/APP/OTA/OTA_prog.c:169
commands
silent
printf "\n@@@ FAILED: HESP8266_u8EnsureConnection() could not reach the server\n"
end

break ../src/APP/OTA/OTA_prog.c:186
commands
silent
printf "\n@@@ ota.txt WAS READ successfully. Raw body:\n<<<%s>>>\n", body
end

break ../src/APP/OTA/OTA_prog.c:182
commands
silent
printf "\n@@@ FAILED: HESP8266_u8HttpGetText(OTA_STATUS_PATH) returned an error\n"
end

break ../src/APP/OTA/OTA_prog.c:193
commands
silent
printf "\n@@@ ota.txt read but version/url/crc32/size fields are missing from: <<<%s>>>\n", body
end

break ../src/APP/OTA/OTA_prog.c:198
commands
silent
printf "\n@@@ parsed fields:\n"
printf "      version = '%s'\n", version_str
printf "      url     = '%s'\n", url_str
printf "      crc32   = '%s'\n", crc_str
printf "      size    = '%s'\n", size_str
printf "    server_version = %u   APP_FW_VERSION = %u\n", server_version, 4
end

break ../src/APP/OTA/OTA_prog.c:209
commands
silent
printf "\n@@@ *** UPDATE FOUND *** server version %u is newer than the installed %u\n", server_version, 4
end

printf "\n=== entering RunOnlineMode() so OTA_vCheckForUpdate() runs ===\n"
set $pc = (void*) 0x08013fb0

printf "\n=== the app is about to run OTA_vCheckForUpdate() over the network ===\n"
continue

detach
quit