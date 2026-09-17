# esp-live.gdb - snapshot of the ESP8266 / USART1 runtime state over ST-Link.
# Usage (OpenOCD server must already listen on 3333):
#   arm-none-eabi-gdb -batch -x tools/esp-live.gdb "Debug/fixed project.elf"
# Addresses come from Debug/fixed project.map:
#   G_u8TcpConnected          0x200009aa
#   G_u8WifiConnected         0x200009ab
#   G_xHESP8266DebugStage     0x200009ac
#   G_u8HESP8266LastStatus    0x200009ad
#   G_u16RxRead               0x2000091c
#   G_u16RxWrite              0x2000091e
#   G_cHESP8266LastResponse   0x20000e40

set pagination off
set confirm off
set height 0
set remotetimeout 20

target extended-remote localhost:3333
monitor halt

echo \n===== ESP8266 / USART1 LIVE STATE =====\n
printf "stage      = "
x/1ub 0x200009ac
printf "lastStatus = "
x/1ub 0x200009ad
printf "wifiConn   = "
x/1ub 0x200009ab
printf "tcpConn    = "
x/1ub 0x200009aa
printf "rxWrite    = "
x/1uh 0x2000091e
printf "rxRead     = "
x/1uh 0x2000091c
printf "\nlast AT line the module sent:\n"
x/s 0x20000e40

echo \n-- USART1  SR / BRR / CR1 --\n
x/1xw 0x40011000
x/1xw 0x40011008
x/1xw 0x4001100c
echo -- GPIOA MODER / AFRH (PA9,PA10 must be 0b10 / 7777) --\n
x/1xw 0x40020000
x/1xw 0x40020024
echo -- RCC CFGR / APB2ENR (SW=HSE, PPRE2=0, USART1EN=bit4) --\n
x/1xw 0x40023808
x/1xw 0x40023844
echo -- NVIC ISER1 (bit5 = USART1 IRQ37) --\n
x/1xw 0xE000E104

echo \n-- where the CPU is right now --\n
info registers pc
bt 8

monitor resume
quit
