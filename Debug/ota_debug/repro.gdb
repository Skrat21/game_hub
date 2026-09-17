set pagination off
set confirm off
set remotetimeout 60
target extended-remote localhost:3333
monitor halt
delete

# --- 1. the OTA trigger-check point ---
break OTA_prog.c:407
commands
silent
printf "\n@@@ STOP A @@@ OTA_prog.c:407 - about to call HESP8266_u8HttpGet(\"/status/ota.txt\")\n"
printf "    G_u8TcpConnected       = %d\n", G_u8TcpConnected
printf "    G_xHESP8266DebugStage  = %d  (3 = MODE_SENT, i.e. HESP8266_vInit() already ran)\n", G_xHESP8266DebugStage
bt 2
continue
end

# --- 2. the driver's connection guard -> returns NOT_CONNECTED ---
break ESP8266_prg.c:286
commands
silent
printf "\n@@@ STOP B @@@ ESP8266_prg.c:286 - taking: return HESP8266_STATUS_NOT_CONNECTED\n"
bt 3
continue
end

# --- 3. the swallowed-error branch ---
break OTA_prog.c:410
commands
silent
printf "\n@@@ STOP C @@@ OTA_prog.c:410 - if (status != HESP8266_STATUS_OK) -> TRUE\n"
printf "    status (local) = %d   (1 = HESP8266_STATUS_NOT_CONNECTED)\n", status
continue
end

# --- 4. the silent early return : STOP HERE (no auto-continue) ---
break OTA_prog.c:415
commands
silent
printf "\n@@@ STOP D @@@ OTA_prog.c:415 - THE SILENT RETURN (no TFT text, no LED, no retry)\n"
bt 3
end

# --- 5. proof of OTA start : STOP HERE if ever reached ---
break OTA_prog.c:440
commands
silent
printf "\n@@@ STOP E @@@ OTA_prog.c:440 - 'UPDATE FOUND' WAS reached (should not happen)\n"
bt 3
end

printf "\n=== live state BEFORE the OTA path ===\n"
print/x G_u8TcpConnected
print/x G_u8WifiConnected
print/x G_xHESP8266DebugStage
print/x G_u8HESP8266LastStatus

printf "\n=== entering RunOnlineMode() (the function main() calls for ONLINE MODE) ===\n"
printf "    its first two actions are HESP8266_vInit() then OTA_vCheckForUpdate() (main.c:712-715)\n"
set $pc = RunOnlineMode
continue

printf "\n=== stopped. final live state ===\n"
print/x G_u8HESP8266LastStatus
printf "G_u8EofReceived     = %d   (0 => download stream never started)\n", G_u8EofReceived
printf "G_u32BytesReceived  = %d\n", G_u32BytesReceived
printf "G_u8TcpConnected    = %d\n", G_u8TcpConnected
printf "G_xHESP8266DebugStage = %d\n", G_xHESP8266DebugStage
info line *$pc

detach
quit
