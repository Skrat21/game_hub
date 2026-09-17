# esp-rx-dump.gdb - dump the full USART1 RX ring buffer (G_u8RxQueue @ 0x2000051c,
# 1024 bytes) so the raw ESP8266 traffic can be inspected offline.
# Usage: arm-none-eabi-gdb -batch -x tools/esp-rx-dump.gdb "Debug/fixed project.elf"

set pagination off
set confirm off
set height 0
set remotetimeout 20

target extended-remote localhost:3333
monitor halt
printf "rxWrite = "
x/1uh 0x2000091e
printf "rxRead  = "
x/1uh 0x2000091c
printf "irqEnabled(CR1 RXNEIE bit5) / SR RXNE bit5 :\n"
x/1xw 0x4001100c
x/1xw 0x40011000
dump binary memory Debug/rx-dump.bin 0x2000051c 0x2000091c
monitor resume
quit
