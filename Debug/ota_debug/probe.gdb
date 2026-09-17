set pagination off
set confirm off
target extended-remote localhost:3333
monitor halt
printf "PC = %p\n", $pc
info registers pc sp
printf "\n--- OTA flag word the code reads @ 0x08004000 ---\n"
x/2xw 0x08004000
printf "\n--- ESP8266 live state ---\n"
print/x G_u8TcpConnected
print/x G_u8WifiConnected
print/x G_xHESP8266DebugStage
print/x G_u8HESP8266LastStatus
printf "\n--- RCC_CSR reset flags @ 0x40023874 ---\n"
x/1xw 0x40023874
monitor resume
detach
quit
