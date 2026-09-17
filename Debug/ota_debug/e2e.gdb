set pagination off
set confirm off
set remotetimeout 300
target extended-remote localhost:3333
monitor reset run
shell ping -n 4 127.0.0.1 > NUL
monitor halt
delete

# stop when the link is up, or on the failure path
break main.c:727
commands
silent
printf "\n@@@ LINK IS UP: main.c:727 - HESP8266_u8EnsureConnection() returned OK\n"
printf "    G_u8TcpConnected      = %d   (1 = TCP link live)\n", G_u8TcpConnected
printf "    G_u8WifiConnected     = %d\n", G_u8WifiConnected
printf "    G_xHESP8266DebugStage = %d   (6 = TCP_READY)\n", G_xHESP8266DebugStage
printf "    last ESP response     = [%s]\n", G_cHESP8266LastResponse
end

break main.c:722
commands
silent
printf "\n@@@ LINK FAILED: main.c:722 - EnsureConnection failed (AP still not reachable)\n"
printf "    G_u8WifiConnected = %d   last ESP response = [%s]   status = %d\n", G_u8WifiConnected, G_cHESP8266LastResponse, G_u8HESP8266LastStatus
end

printf "\n=== entering RunOnlineMode() to bring Wi-Fi/TCP up -> SSID 'Ahmedlaptop'\n"
set $pc = RunOnlineMode
continue

printf "\n=== NOW: real HTTP GET of /status/ota.txt over the live socket, exactly as OTA_vCheckForUpdate does ===\n"
set $rc = (int)HESP8266_u8HttpGet((char*)0x0800e684, (char*)0x20000d88, 64)
printf "\nHESP8266_u8HttpGet(\"/status/ota.txt\", body, 64) -> %d   (0=OK 1=TIMEOUT 2=ERROR 3=NOT_CONNECTED)\n", $rc
printf "G_u8HESP8266LastStatus = %d\n", G_u8HESP8266LastStatus
printf "last line the body reader latched onto = [%s]\n", G_cHESP8266LastResponse
printf "body returned                          = [%s]\n", G_pcUpdateUrl

detach
quit