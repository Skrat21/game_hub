set pagination off
set confirm off
set remotetimeout 300
target extended-remote localhost:3333
monitor halt
delete

printf "OTA_STATUS_PATH literal in flash: "
x/s 0x0800e684

# stop when the link came up, or on the failure path
break main.c:727
commands
silent
printf "\n@@@ STOP: main.c:727 - HESP8266_u8EnsureConnection() returned OK\n"
printf "    G_u8TcpConnected      = %d   (1 = TCP link is live)\n", G_u8TcpConnected
printf "    G_u8WifiConnected     = %d\n", G_u8WifiConnected
printf "    G_xHESP8266DebugStage = %d   (6 = TCP_READY)\n", G_xHESP8266DebugStage
end

break main.c:722
commands
silent
printf "\n@@@ STOP: main.c:722 - EnsureConnection FAILED (no Wi-Fi/TCP available)\n"
printf "    G_u8TcpConnected = %d  G_u8WifiConnected = %d\n", G_u8TcpConnected, G_u8WifiConnected
printf "    last ESP response = [%s]\n", G_cHESP8266LastResponse
printf "    G_u8HESP8266LastStatus = %d\n", G_u8HESP8266LastStatus
end

printf "\n=== entering RunOnlineMode() so its loop brings the Wi-Fi/TCP link up ===\n"
set $pc = RunOnlineMode
continue

printf "\n=== now with a live TCP link: read /status/ota.txt exactly as OTA_vCheckForUpdate does ===\n"
set $rc = (int)HESP8266_u8HttpGet((char*)0x0800e684, (char*)0x20000d88, 64)
printf "\nHESP8266_u8HttpGet(\"/status/ota.txt\", body, 64) -> %d   (0=OK 1=TIMEOUT 2=ERROR 3=NOT_CONNECTED)\n", $rc
printf "G_u8HESP8266LastStatus = %d\n", G_u8HESP8266LastStatus
printf "last line the body reader latched onto = [%s]\n", G_cHESP8266LastResponse
printf "body buffer                            = [%s]\n", G_pcUpdateUrl

detach
quit