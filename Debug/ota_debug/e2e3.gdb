set pagination off
set confirm off
set remotetimeout 300
target extended-remote localhost:3333
monitor reset run
shell ping -n 4 127.0.0.1 > NUL
monitor halt
delete

set {char[12]}0x2000e000  = "Ahmedlaptop"
set {char[10]}0x2000e020  = "katkot123"
set {char[15]}0x2000e040  = "185.176.43.100"
set {char[3]}0x2000e060   = "80"
set {char[16]}0x2000e080  = "/status/rps.txt"
set {char[16]}0x2000e0a0  = "/status/ota.txt"

break main.c:727
commands
silent
printf "\n@@@ link up: G_u8TcpConnected = %d, WiFi = %d\n", G_u8TcpConnected, G_u8WifiConnected
end

printf "\n=== bring the Wi-Fi/TCP link up naturally ===\n"
set $pc = RunOnlineMode
continue

printf "\n################# GET /status/rps.txt (a game status file) #################\n"
call (void)HESP8266_vAbort()
set $c = (int)HESP8266_u8EnsureConnection((char*)0x2000e000, (char*)0x2000e020, (char*)0x2000e040, (char*)0x2000e060, 3)
call (void)MSYSTICK_vSetDelay_ms(300.0)
set {char[9]}0x2000e100 = "SENTINEL"
set $rc = (int)HESP8266_u8HttpGet((char*)0x2000e080, (char*)0x2000e100, 512)
printf "conn=%d   HttpGet -> %d   body = [%s]\n", $c, $rc, (char*)0x2000e100
printf "  last line latched = [%s]   laststatus = %d\n", G_cHESP8266LastResponse, G_u8HESP8266LastStatus

printf "\n################# GET /status/ota.txt (the OTA trigger) #################\n"
call (void)HESP8266_vAbort()
set $c = (int)HESP8266_u8EnsureConnection((char*)0x2000e000, (char*)0x2000e020, (char*)0x2000e040, (char*)0x2000e060, 3)
call (void)MSYSTICK_vSetDelay_ms(300.0)
set {char[9]}0x2000e100 = "SENTINEL"
set $rc = (int)HESP8266_u8HttpGet((char*)0x2000e0a0, (char*)0x2000e100, 512)
printf "conn=%d   HttpGet -> %d   body = [%s]\n", $c, $rc, (char*)0x2000e100
printf "  last line latched = [%s]   laststatus = %d\n", G_cHESP8266LastResponse, G_u8HESP8266LastStatus

detach
quit