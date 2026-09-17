# esp-sample.gdb - one timed sample: state machine + raw RX ring buffer.
# Usage: arm-none-eabi-gdb -batch -x tools/esp-sample.gdb "Debug/fixed project.elf"

set pagination off
set confirm off
set height 0
set remotetimeout 20

target extended-remote localhost:3333
monitor halt
printf "stage="
x/1ub 0x200009ac
printf "status="
x/1ub 0x200009ad
printf "wifi="
x/1ub 0x200009ab
printf "tcp="
x/1ub 0x200009aa
printf "rxW="
x/1uh 0x2000091e
dump binary memory Debug/rx-now.bin 0x2000051c 0x2000091c
monitor resume
quit
