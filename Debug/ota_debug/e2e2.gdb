set pagination off
set confirm off
set remotetimeout 300
target extended-remote localhost:3333
monitor reset run
shell ping -n 4 127.0.0.1 > NUL
monitor halt
delete

# scratch strings + body buffer in the unused heap region (firmware has no malloc)
set {char[12]}0x2000e000  = "Ahmedlaptop"
set {char[10]}0x2000e020  = "katkot123"
set {char[15]}0x2000e040  = "185.176.43.100"
set {char[3]}0x2000e060   = "80"
set {char[17]}0x2000e080  = "/status/main.txt"
set {char[16]}0x2000e0a0  = "/status/ota.txt"

break main.c:727
commands
silent
printf "\n@@@ RunOnlineMode's own EnsureConnection() succeeded: G_u8TcpConnected = %d, WiFi = %d\n", G_u8TcpConnected, G_u8WifiConnected
end

printf "\n=== bring the Wi-Fi/TCP link up naturally via RunOnlineMode() ===\n"
set $pc = RunOnlineMode
continue

printf "\n################# STEP 1: read a GAME status file over the live socket #################\n"
set {char[9]}0x2000e100 = "SENTINEL"
set $rc = (int)HESP8266_u8HttpGet((char*)0x2000e080, (char*)0x2000e100, 512)
printf "HESP8266_u8HttpGet(\"/status/main.txt\") -> %d   (0=OK 1=TIMEOUT 2=ERROR 3=NOT_CONNECTED)\n", $rc
printf "  body = [%s]\n", (char*)0x2000e100

printf "\n--- reconnect (the request sends 'Connection: close') ---\n"
call (void)HESP8266_vAbort()
set $c = (int)HESP8266_u8EnsureConnection((char*)0x2000e000, (char*)0x2000e020, (char*)0x2000e040, (char*)0x2000e060, 3)
printf "EnsureConnection() -> %d   G_u8TcpConnected = %d\n", $c, G_u8TcpConnected

printf "\n################# STEP 2: read the OTA file over the SAME live socket #################\n"
set {char[9]}0x2000e100 = "SENTINEL"
set $rc = (int)HESP8266_u8HttpGet((char*)0x2000e0a0, (char*)0x2000e100, 512)
printf "HESP8266_u8HttpGet(\"/status/ota.txt\")  -> %d   (0=OK 1=TIMEOUT 2=ERROR 3=NOT_CONNECTED)\n", $rc
printf "  body = [%s]\n", (char*)0x2000e100
printf "  last line the reader latched onto = [%s]\n", G_cHESP8266LastResponse

detach
quit