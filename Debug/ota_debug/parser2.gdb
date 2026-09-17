set pagination off
set confirm off
set remotetimeout 180
target extended-remote localhost:3333

# let it boot properly so InitHardware() has run (SysTick is needed by WaitForHttpBody)
monitor reset run
shell ping -n 4 127.0.0.1 > NUL
monitor halt
delete
printf "PC after boot = %p\n", $pc
info line *$pc

printf "G_u8RxQueue = %p   G_u16RxRead = %p   G_u16RxWrite = %p\n", &G_u8RxQueue, &G_u16RxRead, &G_u16RxWrite
printf "body buffer used = 0x20000d88 (G_pcUpdateUrl, 64 bytes)\n"

printf "\n############ CASE 1: body EXACTLY as the server sends it (60 bytes, NO trailing CR/LF) ############\n"
restore "C:/Users/AHMED SHERIF/OneDrive/Desktop/workspace/fixed project/Debug/ota_debug/ota_body_nonl.bin" binary &G_u8RxQueue
set G_u16RxRead = 0
set G_u16RxWrite = 60
set {char[64]}0x20000d88 = "SENTINEL-UNTOUCHED"
set $rc = (int)WaitForHttpBody((char*)0x20000d88, 64)
printf "WaitForHttpBody() -> %d    (0=OK, 1=TIMEOUT)\n", $rc
printf "  last line it latched onto = [%s]\n", G_cHESP8266LastResponse
printf "  body returned             = [%s]\n", G_pcUpdateUrl

printf "\n############ CASE 2: the SAME ota line but WITH a trailing newline ############\n"
restore "C:/Users/AHMED SHERIF/OneDrive/Desktop/workspace/fixed project/Debug/ota_debug/ota_body_nl.bin" binary &G_u8RxQueue
set G_u16RxRead = 0
set G_u16RxWrite = 61
set {char[64]}0x20000d88 = "SENTINEL-UNTOUCHED"
set $rc = (int)WaitForHttpBody((char*)0x20000d88, 64)
printf "WaitForHttpBody() -> %d    (0=OK, 1=TIMEOUT)\n", $rc
printf "  last line it latched onto = [%s]\n", G_cHESP8266LastResponse
printf "  body returned             = [%s]\n", G_pcUpdateUrl

printf "\n############ CASE 3: control - a game status line (what the parser DOES accept) ############\n"
set {char[32]}0x20000510 = "state=DEAD;game=none;time=x\n"
set G_u16RxRead = 0
set G_u16RxWrite = 31
set {char[64]}0x20000d88 = "SENTINEL-UNTOUCHED"
set $rc = (int)WaitForHttpBody((char*)0x20000d88, 64)
printf "WaitForHttpBody() -> %d    (0=OK, 1=TIMEOUT)\n", $rc
printf "  last line it latched onto = [%s]\n", G_cHESP8266LastResponse
printf "  body returned             = [%s]\n", G_pcUpdateUrl

detach
quit