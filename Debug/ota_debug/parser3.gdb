set pagination off
set confirm off
set remotetimeout 180
target extended-remote localhost:3333

# boot normally so InitHardware() has run (SysTick is required by WaitForHttpBody)
monitor reset run
shell ping -n 4 127.0.0.1 > NUL
monitor halt
delete

printf "SysTick CTRL (0xE000E010, bit0 = ENABLE) = "
x/1xw 0xE000E010
printf "G_u8RxQueue = %p   &G_u16RxWrite = %p\n", &G_u8RxQueue, &G_u16RxWrite

printf "\n############ CASE 1: body EXACTLY as the server sends it (60 bytes, NO trailing CR/LF) ############\n"
set {char[60]}0x20000510 = "version=3;url=/firmware/fw_v3.hex;crc32=21657c40;size=171335"
set G_u16RxRead = 0
set G_u16RxWrite = 60
set {char[64]}0x20000d88 = "SENTINEL-UNTOUCHED"
set $rc = (int)WaitForHttpBody((char*)0x20000d88, 64)
printf "WaitForHttpBody() -> %d    (0=OK, 1=TIMEOUT)\n", $rc
printf "  last line it latched onto = [%s]\n", G_cHESP8266LastResponse
printf "  body returned             = [%s]\n", G_pcUpdateUrl

printf "\n############ CASE 2: the SAME ota line but WITH a trailing newline ############\n"
set {char[61]}0x20000510 = "version=3;url=/firmware/fw_v3.hex;crc32=21657c40;size=171335\n"
set G_u16RxRead = 0
set G_u16RxWrite = 61
set {char[64]}0x20000d88 = "SENTINEL-UNTOUCHED"
set $rc = (int)WaitForHttpBody((char*)0x20000d88, 64)
printf "WaitForHttpBody() -> %d    (0=OK, 1=TIMEOUT)\n", $rc
printf "  last line it latched onto = [%s]\n", G_cHESP8266LastResponse
printf "  body returned             = [%s]\n", G_pcUpdateUrl

printf "\n############ CASE 3: control - a game status line (what the parser DOES accept) ############\n"
set {char[28]}0x20000510 = "state=DEAD;game=none;time=x\n"
set G_u16RxRead = 0
set G_u16RxWrite = 28
set {char[64]}0x20000d88 = "SENTINEL-UNTOUCHED"
set $rc = (int)WaitForHttpBody((char*)0x20000d88, 64)
printf "WaitForHttpBody() -> %d    (0=OK, 1=TIMEOUT)\n", $rc
printf "  last line it latched onto = [%s]\n", G_cHESP8266LastResponse
printf "  body returned             = [%s]\n", G_pcUpdateUrl

detach
quit