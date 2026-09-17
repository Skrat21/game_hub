set pagination off
set confirm off
set remotetimeout 180
target extended-remote localhost:3333
monitor halt
delete

printf "G_u8RxQueue = %p   G_u16RxRead = %p   G_u16RxWrite = %p\n", &G_u8RxQueue, &G_u16RxRead, &G_u16RxWrite
printf "body buffer used = 0x20000d88 (G_pcUpdateUrl, 64 bytes)\n"

call (void)MUSART_vDisable_RX_Interrupt()

printf "\n############ CASE 1: body EXACTLY as the server sends it (60 bytes, NO trailing CR/LF) ############\n"
restore "C:/Users/AHMED SHERIF/OneDrive/Desktop/workspace/fixed project/Debug/ota_debug/ota_body_nonl.bin" binary &G_u8RxQueue
set G_u16RxRead = 0
set G_u16RxWrite = 60
set {char[64]}0x20000d88 = "SENTINEL-UNTOUCHED"
set $rc = (int)WaitForHttpBody((char*)0x20000d88, 64)
printf "WaitForHttpBody() -> %d    (0=OK, 1=TIMEOUT)\n", $rc
printf "  last line it latched onto = [%s]\n", G_cHESP8266LastResponse
printf "  body returned             = [%s]\n", G_pcUpdateUrl

printf "\n############ CASE 2: the SAME line but WITH a trailing newline ############\n"
restore "C:/Users/AHMED SHERIF/OneDrive/Desktop/workspace/fixed project/Debug/ota_debug/ota_body_nl.bin" binary &G_u8RxQueue
set G_u16RxRead = 0
set G_u16RxWrite = 61
set {char[64]}0x20000d88 = "SENTINEL-UNTOUCHED"
set $rc = (int)WaitForHttpBody((char*)0x20000d88, 64)
printf "WaitForHttpBody() -> %d    (0=OK, 1=TIMEOUT)\n", $rc
printf "  last line it latched onto = [%s]\n", G_cHESP8266LastResponse
printf "  body returned             = [%s]\n", G_pcUpdateUrl

printf "\n############ CASE 3: a game status line (what the parser DOES accept) ############\n"
restore "C:/Users/AHMED SHERIF/OneDrive/Desktop/workspace/fixed project/Debug/ota_debug/ota_body_nl.bin" binary &G_u8RxQueue
set {char[32]}0x20000528 = "state=DEAD;game=none;time=x\n"
set G_u16RxRead = 0
set G_u16RxWrite = 31
set {char[64]}0x20000d88 = "SENTINEL-UNTOUCHED"
set $rc = (int)WaitForHttpBody((char*)0x20000d88, 64)
printf "WaitForHttpBody() -> %d    (0=OK, 1=TIMEOUT)\n", $rc
printf "  last line it latched onto = [%s]\n", G_cHESP8266LastResponse
printf "  body returned             = [%s]\n", G_pcUpdateUrl

call (void)MUSART_vEnable_RX_Interrupt()
detach
quit