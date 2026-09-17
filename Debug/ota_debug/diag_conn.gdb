set pagination off
set confirm off
set remotetimeout 300

file "Debug/fixed project.elf"
target extended-remote localhost:3333
monitor halt

printf "\n--- where the application is now ---\n"
printf "PC = %p\n", $pc
info line *$pc

printf "\n--- ESP8266 link state ---\n"
printf "G_u8WifiConnected      = %u\n", G_u8WifiConnected
printf "G_u8TcpConnected       = %u\n", G_u8TcpConnected
printf "G_xHESP8266DebugStage  = %u\n", G_xHESP8266DebugStage
printf "   (0=RESET 1=USART_READY 2=ATE0_SENT 3=MODE_SENT 4=WIFI_SENT 5=WIFI_READY 6=TCP_READY 7=MAIN_STATUS_SENT)\n"
printf "G_u8HESP8266LastStatus = %u  (0=OK 1=TIMEOUT 2=ERROR 3=NOT_CONNECTED)\n", G_u8HESP8266LastStatus

printf "\n--- last raw response captured from the ESP8266 ---\n"
x/s G_cHESP8266LastResponse

detach
quit